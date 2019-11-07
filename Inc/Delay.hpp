#ifndef DELAY_H
#define DELAY_H

#include "Effect.hpp"
#include "DelayBlock.hpp"
#include "arm_math.h"
#include "mymath.h"

class Delay: public Effect{
    private:
        DelayBlock delay_left;
        DelayBlock delay_right;
        float32_t feedback;
        float32_t dry_level;
        float32_t wet_level;

    public:
        Delay(float32_t feedback, float32_t dry_level, float32_t wet_level);
        void ProcessBlock(float32_t *pData, uint32_t block_size);

};

#endif