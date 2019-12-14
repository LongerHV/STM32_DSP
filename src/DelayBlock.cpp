#include "DelayBlock.h"

DelayBlock::DelayBlock(float32_t *pData, uint32_t max_delay, uint32_t offset) {
    this->pData = pData;
    this->max_delay = max_delay;
    this->offset = offset;
    this->current = 0;
    this->ResetBuffer();
}

uint32_t DelayBlock::GetMaxDelay(){
    return this->max_delay;
}

void DelayBlock::GetTailBlock(float32_t *pDst, uint32_t block_size) {
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

void DelayBlock::FeedBlock(float32_t *pSrc, uint32_t block_size) {
    for (uint32_t i = 0; i < block_size; i++) {
        this->pData[this->current] = pSrc[i];
        if (++this->current >= this->max_delay) {
            this->current = 0;
        }
    }
}

void DelayBlock::ResetBuffer() {
    arm_fill_f32(0, this->pData, this->max_delay);
}

float32_t DelayBlock::GetSample(uint32_t offset, uint32_t index){
    uint32_t tail_index;
    uint32_t current = this->current;
    current += index;
    if(current >= this->max_delay){
        current -= this->max_delay;
    }
    if(current >= offset){
        tail_index = current - offset;
    } else {
        tail_index = this->max_delay + current - offset;
    }
    return this->pData[tail_index];
}

void DelayBlock::SetCurrent(uint32_t current){
    this->current = current;
}
