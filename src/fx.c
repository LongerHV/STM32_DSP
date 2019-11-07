#include "fx.h"

// functions implementation
float32_t math_atan(float32_t x){
    return arm_cos_f32(x) / arm_sin_f32(x);
}

float32_t math_sin2(float32_t x){
    return pow(arm_sin_f32(x), 2);
}

void fx_uint16_to_float(uint16_t *pSrc, float32_t *pDst, uint32_t blockSize){
    for(uint32_t i = 0; i < blockSize; i++)
        pSrc[i] ^= (uint16_t)1 << 15;
    arm_q15_to_float((int16_t *)pSrc, pDst, blockSize);
}

void fx_float_to_uint16(float32_t *pSrc, uint16_t *pDst, uint32_t blockSize){
    arm_float_to_q15(pSrc, (int16_t *)pDst, blockSize);
    for(uint32_t i = 0; i < blockSize; i++)
        pDst[i] ^= (uint16_t)1 << 15;
}

void fx_float_scalar_mul(float32_t *pSrc, float32_t *pDst, float32_t scalar, uint32_t blockSize){
    for(uint32_t i = 0; i < blockSize; i++){
        pDst[i] = pSrc[i] * scalar;
    }
}

// void fx_delay_init(FX_DelayTypeDef *delay, uint32_t size, uint32_t offset, float32_t feedback, float32_t dry, float32_t wet, float32_t *pData){
void fx_delay_init(FX_DelayTypeDef *delay, uint32_t size, uint32_t offset, q15_t *pData){
    delay->pData = pData;
    delay->MaxSize = size;
    delay->Offset = offset;
    delay->Index = 0;
    // delay->Feedback = feedback;
    // delay->DryLevel = dry;
    // delay->WetLevel = wet;
}

void fx_delay(FX_DelayTypeDef *delay, float32_t *pSrc, float32_t *pDst, uint32_t blockSize){

}

void fx_delayChannel(FX_DelayTypeDef *delay, float32_t *pData, float32_t dry, float32_t wet, float32_t fb, uint32_t blockSize){
    float32_t *tailFloat = (float32_t *) malloc(blockSize * sizeof(float32_t));
    float32_t *feedback = (float32_t *) malloc(blockSize * sizeof(float32_t));
    q15_t *tempFixed = (q15_t *) malloc(blockSize * sizeof(q15_t));

    // Get tail blocks
    fx_delayGetTail(delay, tempFixed, blockSize);

    // Convert tail blocks from fixed to floating point format
    arm_q15_to_float(tempFixed, tailFloat, blockSize);

    // Prepare feedback block
    fx_float_scalar_mul(tailFloat, feedback, fb, blockSize);

    arm_add_f32(pData, feedback, feedback, blockSize);

    // Convert feedback block to fixed point
    arm_float_to_q15(feedback, tempFixed, blockSize);

    // Feed delay block
    fx_delayFeed(delay, tempFixed, blockSize);

    // Dry gain
    fx_float_scalar_mul(pData, pData, dry, blockSize);

    // Wet gain
    fx_float_scalar_mul(tailFloat, tailFloat, wet, blockSize);

    // Add Wet and Dry signals
    arm_add_f32(pData, tailFloat, pData, blockSize);

    free(tailFloat);
    free(feedback);
    free(tempFixed);
}

void fx_delaySrereo(FX_DelayStereoTypeDef *delay, float32_t *pDataL, float32_t *pDataR, uint32_t blockSize){
    fx_delayChannel(delay->delayLeft, pDataL, delay->DryLevel, delay->WetLevel, delay->WetLevel, blockSize);
    fx_delayChannel(delay->delayRight, pDataR, delay->DryLevel, delay->WetLevel, delay->WetLevel, blockSize);
}

void fx_delayGetTail(FX_DelayTypeDef *delay, q15_t *pDst, uint32_t blockSize){
    uint32_t tail;
    for(uint32_t i = 0; i < blockSize; i++){
        if(delay->Offset > delay->Index + i){
            tail = delay->MaxSize - delay->Offset + delay->Index + i;
        } else {
            tail = delay->Index - delay->Offset + i;
        }
        pDst[i] = delay->pData[tail];
    }
}

void fx_delayFeed(FX_DelayTypeDef *delay, q15_t *pSrc, uint32_t blockSize){
    for(uint32_t i = 0; i < blockSize; i++){
        delay->pData[delay->Index] = pSrc[i];
        if(++delay->Index >= delay->MaxSize){
            delay->Index = 0;
        }
    }
}
