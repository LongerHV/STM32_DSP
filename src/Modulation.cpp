#include "Modulation.h"

Modulation::Modulation(const char *name, DelayBlock *left_delay, DelayBlock *right_delay) {
    this->SetName(name);
    this->left_delay = left_delay;
    this->right_delay = right_delay;
    this->max_delay = left_delay->GetMaxDelay();

    this->number_of_parameters = 6;
    this->current_parameter = 0;
    this->parameters[0] = new Parameter("Rate", "Hz", &this->rate, 0.5f, 10.0f, 0.1f, 1.0f);
    this->parameters[1] = new Parameter("Depth", "%", &this->depth, 0.0f, 100.0f, 5.0f, 25.0f);
    this->parameters[2] = new Parameter("Delay", "ms", &this->delay_time, 1.0f, 30.0f, 1.0f, 10.0f);
    this->parameters[3] = new Parameter("Feedback", "", &this->feedback, 0.0f, 1.0f, 0.1, 0.25f);
    this->parameters[4] = new Parameter("Dry level", "", &this->dry_level, 0.0f, 1.0f, 0.1f, 0.8f);
    this->parameters[5] = new Parameter("Wet level", "", &this->wet_level, 0.0f, 1.0f, 0.1f, 0.8f);
    for (uint8_t i = 6; i < 10; i++) parameters[i] = NULL;

    this->UpdateParameters();
}

Modulation::~Modulation() {
}

void Modulation::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
}

void Modulation::UpdateParameters() {
    this->rate = this->parameters[0]->GetValue();
    this->depth = this->parameters[1]->GetValue();
    this->delay_time = this->parameters[2]->GetValue();
    this->feedback = this->parameters[3]->GetValue();
    this->dry_level = this->parameters[4]->GetValue();
    this->wet_level = this->parameters[5]->GetValue();
}

float32_t GetLFO() {
    return 0;
}