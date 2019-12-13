#include "DelayBlock.h"

DelayBlock::DelayBlock(q15_t *pData, uint32_t max_delay, uint32_t offset) {
    this->pData = pData;
    this->max_delay = max_delay;
    this->offset = offset;
    this->current = 0;
    this->ResetBuffer();
}

uint32_t DelayBlock::GetMaxDelay(){
    return this->max_delay;
}

void DelayBlock::GetTailBlock(q15_t *pDst, uint32_t block_size) {
    uint32_t tail_index;
    for (uint32_t i = 0; i < block_size; i++) {
        if (this->offset > this->current + i) {
            tail_index = this->max_delay - this->offset + this->current + i;
        } else {
            tail_index = this->current - this->offset + i;
        }
        pDst[i] = this->pData[tail_index];
    }
}

void DelayBlock::FeedBlock(q15_t *pSrc, uint32_t block_size) {
    for (uint32_t i = 0; i < block_size; i++) {
        this->pData[this->current] = pSrc[i];
        if (++this->current >= this->max_delay) {
            this->current = 0;
        }
    }
}

void DelayBlock::ResetBuffer() {
    arm_fill_q15(0, this->pData, this->max_delay);
}

q15_t DelayBlock::GetSample(uint32_t offset, uint32_t index){
    uint32_t tail_index;
    if (offset > this->current + index) {
        tail_index = this->max_delay - offset + this->current + index;
    } else {
        tail_index = this->current - offset + index;
    }
    return this->pData[tail_index];
}
