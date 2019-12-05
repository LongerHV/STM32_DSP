#ifndef MYMATH_H
#define MYMATH_H

#include "arm_math.h"
#include "main.h"

float32_t arm_atg(float32_t x);
float32_t arm_sin2(float32_t x);

void arm_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize);
void arm_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize);

#endif