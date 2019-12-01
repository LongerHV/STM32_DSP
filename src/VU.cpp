#include "VU.hpp"

VU::VU(uint8_t origin_y, uint8_t origin_x, Display *disp){
    this->origin_y = origin_y;
    this->origin_x = origin_x;
    this->my_disp = disp;
}

void VU::Update(float32_t *data, uint32_t block_size){
    uint16_t colour;
    arm_rms_f32(data, block_size, &this->rms);
    this->rms_sum += this->rms;
    if(this->counter++ >= 100){
        if(this->rms_sum < this->rms_sum_prev) this->rms_sum = 0.95 * this->rms_sum_prev;
        this->rms_sum_prev = this->rms_sum;

        for(uint8_t i = 0; i < 5; i++){
            colour = i < 3 ? GREEN : (i == 3 ? YELLOW : RED);
            if(this->rms_sum >= 8){
                this->my_disp->PushChar(this->origin_y - i, this->origin_x, 0x0012, colour);
                this->rms_sum -= 8;
            }else{
                this->my_disp->PushChar(this->origin_y - i, this->origin_x, 0x000A + ((uint8_t)this->rms_sum <= 8 ? (uint8_t)(this->rms_sum) : 8), colour);
                this->rms_sum = 0.0;
            }
        }
        this->rms_sum = 0.0;
        this->counter = 0;
    }
}
