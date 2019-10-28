#ifndef __FX_H
#define __FX_H
#ifdef __cplusplus
extern "C"
{
#endif

// Includes
#include "main.h"
#include "math.h"

// Defines

// Instance structures
typedef struct{
    uint16_t MaxSize;
    uint16_t Offset;
    uint16_t Index;
    float32_t Feedback;
    float32_t Wet;
    float32_t *pData;
} FX_DelayTypeDef;

// Function prototypes
void FX_DELAY(FX_DelayTypeDef *delay, float32_t *pSource, float32_t *pDest, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif