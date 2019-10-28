#include "fx.h"

// functions implementation
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

void fx_delay(FX_DelayTypeDef *delay, float32_t *pSrc, uint32_t blockSize){
    uint16_t tail;
    for(uint16_t i = 0; i < blockSize; i++){
        // Calculate index of delay tail
        if(delay->Offset > delay->Index){
            tail = delay->MaxSize - delay->Offset + delay->Index;
        } else {
            tail = delay->Index - delay->Offset;
        }

        // Calculate value for delay input sample
        delay->pData[delay->Index] = pSrc[i] + (delay->Feedback * delay->pData[delay->Index]);

        // Calculate value for output sample
        pSrc[i] += (delay->Wet * delay->pData[tail]);

        // Increment delay counter
        if(++delay->Index >= delay->MaxSize){
            delay->Index = 0;
        }
    }
}


