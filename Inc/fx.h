#ifndef __FX_H
#define __FX_H
#ifdef __cplusplus
extern "C"
{
#endif

// Includes
#include "main.h"
#include "arm_math.h"
#include "math.h"

// Defines
#define LFO_SIN 0;
#define LFO_SQUARE 1;
#define LFO_TRIANGLE 2;
#define LFO_SAW_RISING 3;
#define LFO_SAW_FALLING 4;

// Instance structures

typedef struct{
    uint32_t MaxSize;
    uint32_t Offset;
    uint32_t Index;
    // float32_t *pData;
    q15_t *pData;
} FX_DelayTypeDef;

typedef struct{
    FX_DelayTypeDef *delayLeft;
    FX_DelayTypeDef *delayRight;
    float32_t Feedback;
    float32_t DryLevel;
    float32_t WetLevel;
} FX_DelayStereoTypeDef;

typedef struct{
    uint32_t Waveform;
    float32_t Frequency;
    float32_t Depth;
    float32_t Angle;
    float32_t Amplitude;
} FX_LfoTypeDef;

typedef struct{
    FX_DelayTypeDef *Delay;
    FX_LfoTypeDef *LFO;
    float32_t DryLevel;
    float32_t WetLevel;
    uint32_t ModType;
} FX_ModulationTypeDef;

// Function prototypes
// Math functions
float32_t math_atg(float32_t x);
float32_t math_sin2(float32_t x);

// Data conversion
void fx_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize);
void fx_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize);
void fx_float_scalar_mul(float32_t *pSrc, float32_t *pDst, float32_t scalar, uint32_t blockSize);

// Delay functions
// void fx_delay_init(FX_DelayTypeDef *delay, uint32_t size, uint32_t offset, float32_t feedback, float32_t dry, float32_t wet, float32_t *pData);
void fx_delay_init(FX_DelayTypeDef *delay, uint32_t size, uint32_t offset, q15_t *pData);
void fx_delay(FX_DelayTypeDef *delay, float32_t *pSrc, float32_t *pDst, uint32_t blockSize);
void fx_delayChannel(FX_DelayTypeDef *delay, float32_t *pData, float32_t dry, float32_t wet, float32_t fb, uint32_t BlockSize);
void fx_delaySrereo(FX_DelayStereoTypeDef *delay, float32_t *pDataL, float32_t *pDataR, uint32_t blockSize);
void fx_delayGetTail(FX_DelayTypeDef *delay, q15_t *pDst, uint32_t blockSize);
void fx_delayFeed(FX_DelayTypeDef *delay, q15_t *pSrc, uint32_t blockSize);

// Modulation functions

#ifdef __cplusplus
}
#endif
#endif