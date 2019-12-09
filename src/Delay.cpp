#include "Delay.h"

Delay::Delay(const char *name, DelayBlock *delay_left, DelayBlock *delay_right) {
    this->SetName(name);
    this->delay_left = delay_left;
    this->delay_right = delay_right;
    this->number_of_parameters = 4;
    this->current_parameter = 0;

    this->parameters[0] = new Parameter("Time", "s", &this->delay_time, 0.0f, 1.0f, 0.05f, 0.5f);
    this->parameters[1] = new Parameter("Feedback", "", &this->feedback, 0.0f, 1.0f, 0.05f, 0.75f);
    this->parameters[2] = new Parameter("Dry level", "", &this->dry_level, 0.0f, 1.0f, 0.05f, 0.8f);
    this->parameters[3] = new Parameter("Wet level", "", &this->wet_level, 0.0f, 1.0f, 0.05f, 0.5f);
    for (uint8_t i = 4; i < 10; i++) parameters[i] = NULL;

    this->UpdateParameters();
}

Delay::~Delay() {
}

void Delay::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
    float32_t *temp_float = new float32_t[block_size];
    float32_t *feedback_block = new float32_t[block_size];
    q15_t *temp_fixed = new q15_t[block_size];
    float32_t *pData;
    DelayBlock *channel;
    for (uint8_t i = 0; i < 2; i++) {
        if (i == 0) {
            channel = this->delay_left;
            pData = pData_left;
        } else {
            channel = this->delay_right;
            pData = pData_right;
        }

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
    delete[] temp_float;
    delete[] temp_fixed;
    delete[] feedback_block;
}

void Delay::UpdateParameters() {
    this->delay_left->offset = this->parameters[0]->GetValue() * SAMPLING_RATE;
    this->feedback = this->parameters[1]->GetValue();
    this->dry_level = this->parameters[2]->GetValue();
    this->wet_level = this->parameters[3]->GetValue();
}