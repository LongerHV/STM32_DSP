#include "Drive.h"

Drive::Drive(const char *name) {
    this->SetName(name);
    this->number_of_parameters = 2;
    this->current_parameter = 0;

    this->parameters[0] = new Parameter("Drive", " ", 5, 100, 5, 10);
    this->parameters[1] = new Parameter("Volume", "%", 0, 100, 5, 100);
    for (uint8_t i = 2; i < 10; i++) parameters[i] = NULL;

    this->UpdateParameters();
}

Drive::~Drive() {

}

void Drive::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size){
    for(uint32_t i = 0; i < block_size; i++){
        pData_left[i] = 2 * this->volume * atan(this->drive * pData_left[i]) * ONE_OVER_PI;
        pData_right[i] = 2 * this->volume * atan(this->drive * pData_right[i]) * ONE_OVER_PI;
    }
}

void Drive::UpdateParameters(){
    this->drive = this->parameters[0] ->GetValue();
    this->volume = (float32_t)this->parameters[1]->GetValue() * 0.01f;
}
