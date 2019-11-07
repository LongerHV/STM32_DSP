#ifndef DELAYBLOCK_H
#define DELAYBLOCK_H

#include "DelayBlock.hpp"
#include "arm_math.h"
#include "mymath.h"

class DelayBlock{
    private:
        q15_t *pData;
        uint32_t max_delay;
        uint32_t offset;
        uint32_t current;
        friend class Delay;

    public:
        DelayBlock();
        void GetTailBlock(q15_t *pDst, uint32_t block_size);
        void Feed(q15_t *pSrc, uint32_t block_size);

};

#endif