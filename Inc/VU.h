#ifndef VU_H
#define VU_H

#include "Display.h"
#include "arm_math.h"
#include "mymath.h"
#include "main.h"

class VU {
   private:
    Display *my_disp;
    // float32_t rms = 0.0;
    // float32_t rms_sum = 0.0;
    // float32_t rms_sum_prev = 0.0;
    float32_t peak = 0.0f;
    float32_t peak_prev = 0.0f;
    uint8_t counter = 0;
    uint8_t origin_y, origin_x;

   public:
    VU(uint8_t origin_y, uint8_t origin_x, Display *disp);
    void Update(float32_t *data, uint32_t block_size);
};

#endif