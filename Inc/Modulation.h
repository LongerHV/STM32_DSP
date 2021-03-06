#ifndef MODULATION_H
#define MODULATION_H

#include "DelayBlock.h"
#include "Effect.h"
#include "arm_math.h"
#include "mymath.h"
#include "main.h"

#define LFO_NONE (uint8_t)0
#define LFO_SINUSOIDAL (uint8_t)1
#define LFO_TRIANGULAR (uint8_t)2
#define LFO_SAWTOOTH_P (uint8_t)3
#define LFO_SAWTOOTH_N (uint8_t)4

class Modulation : public Effect {
   private:
    // parameters
    float32_t wet_level;
    float32_t dry_level;
    float32_t feedback;
    float32_t depth;
    float32_t rate;

    // variables
    DelayBlock *delay_left;
    DelayBlock *delay_right;
    float32_t phi;
    uint32_t max_delay;
    uint8_t type;

   public:
    Modulation(const char *name, DelayBlock *delay_left, DelayBlock *delay_right);
    ~Modulation();
    void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);
    void UpdateParameters();
    float32_t GetLFO();
};

#endif