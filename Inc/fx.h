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

// Instance structures
typedef struct{
    uint32_t MaxSize;
    uint32_t Offset;
    uint32_t Index;
    float32_t Feedback;
    float32_t Wet;
    float32_t *pData;
} FX_DelayTypeDef;

// Function prototypes
void fx_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize);
void fx_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize);

void fx_delay_init(FX_DelayTypeDef *delay, uint32_t size, uint32_t offset, float32_t feedback, float32_t wet, float32_t *pData);
void fx_delay(FX_DelayTypeDef *delay, float32_t *pSrc, uint32_t blockSize);

#ifdef __cplusplus
}
#endif
#endif