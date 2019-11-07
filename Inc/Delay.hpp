#ifndef DELAY_H
#define DELAY_H

#include "Effect.hpp"
#include "arm_math.h"

class Delay: public Effect{
    private:
        float32_t feedback;
        float32_t dry_level;
        float32_t wet_level;

    public:
        Delay(float32_t feedback, float32_t dry_level, float32_t wet_level);
        void ProcessBlock(uint32_t block_size);

};

#endif