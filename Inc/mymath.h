#ifndef MYMATH_H
#define MYMATH_H

#include "arm_math.h"

float32_t arm_atg(float32_t x);
float32_t arm_sin2(float32_t x);
void arm_mat_scalar_mul_f32(float32_t *pSrc, float32_t *pDst, float32_t scalar, uint32_t blockSize);

void arm_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize);
void arm_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize);

#endif