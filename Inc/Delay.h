#ifndef DELAY_H
#define DELAY_H

#include "DelayBlock.h"
#include "Effect.h"
#include "arm_math.h"
#include "mymath.h"
#include "main.h"

class Delay : public Effect {
   private:
    DelayBlock *delay_left;
    DelayBlock *delay_right;
    float32_t delay_time;
    float32_t feedback;
    float32_t dry_level;
    float32_t wet_level;

   public:
    Delay(const char *name, DelayBlock *delay_left, DelayBlock *delay_right);
    ~Delay();
    void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);
    void UpdateParameters();
};

#endif