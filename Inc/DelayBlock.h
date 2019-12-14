#ifndef DELAYBLOCK_H
#define DELAYBLOCK_H

#include "arm_math.h"
#include "mymath.h"
#include "main.h"

class DelayBlock {
   private:
    float32_t *pData;
    uint32_t max_delay;
    uint32_t offset;
    uint32_t current;
    friend class Delay;
    friend class Modulation;

   public:
    DelayBlock(float32_t *pData, uint32_t max_delay, uint32_t offset);
    uint32_t GetMaxDelay();
    void SetOffset(uint32_t offset);
    void SetCurrent(uint32_t current);
    void GetTailBlock(float32_t *pDst, uint32_t block_size);
    void FeedBlock(float32_t *pSrc, uint32_t block_size);
    void ResetBuffer();
    float32_t GetSample(uint32_t offset, uint32_t index);
};

#endif