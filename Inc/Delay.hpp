#ifndef DELAY_H
#define DELAY_H

#include "Effect.hpp"
#include "DelayBlock.hpp"
#include "arm_math.h"
#include "mymath.h"

class Delay: public Effect{
// class Delay{
    private:
        DelayBlock *delay_left;
        DelayBlock *delay_right;
        float32_t feedback;
        float32_t dry_level;
        float32_t wet_level;

    public:
        Delay(const char *name, DelayBlock *delay_left, DelayBlock *delay_right, float32_t feedback, float32_t dry_level, float32_t wet_level);
        ~Delay();
        void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);

};

#endif