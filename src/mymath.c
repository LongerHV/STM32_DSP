#include "mymath.h"

float32_t arm_atg(float32_t x){
    return arm_cos_f32(x) / arm_sin_f32(x);
}

float32_t arm_sin2(float32_t x){
    return pow(arm_sin_f32(x), 2);
}

void arm_mat_scalar_mul_f32(float32_t *pSrc, float32_t *pDst, float32_t scalar, uint32_t blockSize){
    for(uint32_t i = 0; i < blockSize; i++){
        pDst[i] = pSrc[i] * scalar;
    }
}

void arm_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize){
    for(uint32_t i = 0; i < blockSize; i++)
        pSrc[i] ^= (uint16_t)1 << 15;
    arm_q15_to_float((int16_t *)pSrc, pDst, blockSize);
}

void arm_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize){
    arm_float_to_q15(pSrc, (int16_t *)pDst, blockSize);
    for(uint32_t i = 0; i < blockSize; i++)
        pDst[i] ^= (uint16_t)1 << 15;
}