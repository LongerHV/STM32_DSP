#ifndef DELAYBLOCK_H
#define DELAYBLOCK_H

#include "arm_math.h"
#include "mymath.h"
#include "main.h"

class DelayBlock {
   private:
    q15_t *pData;
    uint32_t max_delay;
    uint32_t offset;
    uint32_t current;
    friend class Delay;
    friend class Modulation;

   public:
    DelayBlock(q15_t *pData, uint32_t max_delay, uint32_t offset);
    uint32_t GetMaxDelay();
    void SetOffset(uint32_t offset);
    void GetTailBlock(q15_t *pDst, uint32_t block_size);
    void FeedBlock(q15_t *pSrc, uint32_t block_size);
    void ResetBuffer();
    q15_t GetSample(uint32_t offset, uint32_t index);
};

#endif