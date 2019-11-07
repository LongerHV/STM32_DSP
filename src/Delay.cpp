#include "Delay.hpp"

Delay::Delay(float32_t feedback, float32_t dry_level, float32_t wet_level){
    this->feedback = feedback;
    this->dry_level = dry_level;
    this->wet_level = wet_level;
}

void Delay::ProcessBlock(float32_t *pData, uint32_t block_size){
    float32_t *temp_float = (float32_t *) malloc(block_size * sizeof(float32_t));
    float32_t *feedback_block = (float32_t *) malloc(block_size * sizeof(float32_t));
    q15_t *temp_fixed = (q15_t *) malloc(block_size * sizeof(q15_t));
    DelayBlock *channel;
    for(uint8_t i = 0; i < 2; i++){
        if(i == 0) channel = &this->delay_left;
        else channel = &this->delay_right;

        // Get tail blocks
        channel->GetTailBlock(temp_fixed, block_size);
        // Convert tail blocks from fixed to floating point format
        arm_q15_to_float(temp_fixed, temp_float, block_size);
        // Prepare feedback block
        arm_scale_f32(temp_float, this->feedback, feedback_block, block_size);
        arm_add_f32(pData, feedback_block, feedback_block, block_size);
        // Convert feedback block to fixed point
        arm_float_to_q15(feedback_block, temp_fixed, block_size);
        // Feed delay block
        channel->Feed(temp_fixed, block_size);
        // Dry gain
        arm_scale_f32(pData, this->dry_level, pData, block_size);
        // Wet gain
        arm_scale_f32(temp_float, this->wet_level, temp_float, block_size);
        // Add Wet and Dry signals
        arm_add_f32(pData, temp_float, pData, block_size);
    }
    free(temp_float);
    free(temp_fixed);
    free(feedback_block);
}
