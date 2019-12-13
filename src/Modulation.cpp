#include "Modulation.h"

Modulation::Modulation(const char *name, DelayBlock *delay_left, DelayBlock *delay_right) {
    this->SetName(name);
    this->delay_left = delay_left;
    this->delay_right = delay_right;
    this->max_delay = delay_left->GetMaxDelay();
    this->phi = 0.0f;

    this->number_of_parameters = 6;
    this->current_parameter = 0;
    this->parameters[0] = new Parameter("Rate", "Hz", 500, 10000, 100, 1000);
    this->parameters[1] = new Parameter("Depth", "%", 0, 100, 5, 0);
    this->parameters[2] = new Parameter("Delay", "s", 1, 30, 1, 10);
    this->parameters[3] = new Parameter("Feedback", "%", 0, 100, 5, 0);
    this->parameters[4] = new Parameter("Dry level", "%", 0, 100, 5, 0);
    this->parameters[5] = new Parameter("Wet level", "%", 0, 100, 5, 80);
    for (uint8_t i = 6; i < 10; i++) parameters[i] = NULL;

    this->UpdateParameters();
}

Modulation::~Modulation() {
}

void Modulation::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
    uint32_t offset;
    float32_t *LFO = new float32_t[block_size];
    float32_t *temp_float = new float32_t[block_size];
    float32_t *feedback_block = new float32_t[block_size];
    q15_t *temp_fixed = new q15_t[block_size];

    // Calulate LFO value for every sample
    for(uint32_t i = 0; i < block_size; i++){
        LFO[i] = arm_sin_f32(this->phi);
        this->phi += this->rate * 2 * PI / (float32_t)SAMPLING_RATE;
        if(this->phi >= 2 * PI)
            this->phi = 0;
    }

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

        // Get tail block
        for(uint32_t i = 0; i < block_size; i++){
            offset = channel->offset * (1 + (LFO[i] * this->depth));
            temp_fixed[i] = channel->GetSample(offset, i);
        }

        // Convert tail block from fixed to floating point format
        arm_q15_to_float(temp_fixed, temp_float, block_size);

        // Prepare feedback block
        arm_scale_f32(temp_float, this->feedback, feedback_block, block_size);
        arm_add_f32(pData, feedback_block, feedback_block, block_size);

        // Convert feedback block to fixed point
        arm_float_to_q15(feedback_block, temp_fixed, block_size);

        // Feed delay block
        channel->FeedBlock(temp_fixed, block_size);

        // Dry gain
        arm_scale_f32(pData, this->dry_level, pData, block_size);

        // Wet gain
        arm_scale_f32(temp_float, this->wet_level, temp_float, block_size);

        // Add Wet and Dry signals
        arm_add_f32(pData, temp_float, pData, block_size);
    }

    delete[] LFO;
    delete[] temp_float;
    delete[] temp_fixed;
    delete[] feedback_block;
}

void Modulation::UpdateParameters() {
    this->rate = this->parameters[0]->GetValue() * 0.001;
    this->depth = this->parameters[1]->GetValue() * 0.01;
    this->delay_left->offset = this->parameters[2]->GetValue() * SAMPLING_RATE * 0.001;
    this->delay_right->offset = this->parameters[2]->GetValue() * SAMPLING_RATE * 0.001;
    this->feedback = this->parameters[3]->GetValue() * 0.01;
    this->dry_level = this->parameters[4]->GetValue() * 0.01;
    this->wet_level = this->parameters[5]->GetValue() * 0.01;
}

float32_t GetLFO() {
    return 0;
}