#ifndef BIQUAD_H
#define BIQUAD_H

#include "Effect.h"
#include "arm_math.h"
#include "main.h"
#include "mymath.h"

#define LOWPASS (uint8_t)0
#define HIGHPASS (uint8_t)1
#define BANDPASS (uint8_t)2
#define PEAK (uint8_t)3
#define NOTCH (uint8_t)4
#define LOWSHELF (uint8_t)5
#define HIGHSHELF (uint8_t)6

class Biquad : public Effect {
   private:
    arm_biquad_cascade_df2T_instance_f32 *filter_instance_L;
    arm_biquad_cascade_df2T_instance_f32 *filter_instance_R;
    float32_t Fs, Fc, Q, peak_gain, type;

   public:
    Biquad(const char *name);
    ~Biquad();
    void RecalculateCoeffitients();
    void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);
    void UpdateParameters();
};

#endif