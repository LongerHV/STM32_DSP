#include "VU.h"

VU::VU(uint8_t origin_y, uint8_t origin_x, Display *disp) {
    this->origin_y = origin_y;
    this->origin_x = origin_x;
    this->my_disp = disp;
}

void VU::Update(float32_t *data, uint32_t block_size) {
    float32_t max, min;
    uint32_t dummy;
    arm_max_f32(data, block_size, &max, &dummy);
    arm_min_f32(data, block_size, &min, &dummy);
    if (max > this->peak) 
        this->peak = max;
    if (-min > this->peak)
        this->peak = -min;
    if (this->counter++ >= 4) {
        uint16_t colour;
        this->peak *= 40;
        if (this->peak < this->peak_prev) this->peak = 0.90 * this->peak_prev;
        this->peak_prev = this->peak;

        for (uint8_t i = 0; i < 5; i++) {
            colour = i < 3 ? GREEN : (i == 3 ? YELLOW : RED);
            if (this->peak >= 8) {
                this->my_disp->PushChar(this->origin_y - i, this->origin_x, 0x0012, colour);
                this->peak -= 8;
            } else {
                this->my_disp->PushChar(this->origin_y - i, this->origin_x, 0x000A + ((uint8_t)this->peak <= 8 ? (uint8_t)(this->peak) : 8), colour);
                this->peak = 0.0;
            }
        }
        this->peak = 0.0;
        this->counter = 0;
    }
}
