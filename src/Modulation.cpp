#include "Modulation.h"

Modulation::Modulation(const char *name, DelayBlock *left_delay, DelayBlock *right_delay, float32_t wet_level, float32_t dry_level, float32_t feedback, float32_t depth, float32_t rate, uint32_t delay) {
    this->SetName(name);
    this->left_delay = left_delay;
    this->right_delay = right_delay;
    this->wet_level = wet_level;
    this->dry_level = dry_level;
    this->feedback = feedback;
    this->depth = depth;
    this->rate = rate;
    this->max_delay = delay;
    this->delay = 0.5 * this->max_delay;

    this->number_of_parameters = 6;
    this->current_parameter = 0;
    this->parameters[0] = new Parameter_float32("Rate", &this->rate, 10.0);
    this->parameters[1] = new Parameter_float32("Depth", &this->depth, 1.0);
    this->parameters[2] = new Parameter_uint32("Delay", &this->delay, this->max_delay);
    this->parameters[3] = new Parameter_float32("Feedback", &this->feedback, 1.0);
    this->parameters[4] = new Parameter_float32("Dry level", &this->dry_level, 1.0);
    this->parameters[5] = new Parameter_float32("Wet level", &this->wet_level, 1.0);
    for (uint8_t i = 6; i < 10; i++) parameters[i] = NULL;
}

Modulation::~Modulation() {
}

void Modulation::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
}

float32_t GetLFO() {
    return 0;
}