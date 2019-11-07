#include "DelayBlock.hpp"

DelayBlock::DelayBlock(){

}

void DelayBlock::GetTailBlock(q15_t *pDst, uint32_t block_size){
    uint32_t tail_index;
    for(uint32_t i = 0; i < block_size; i++){
        if(this->offset > this->current + i){
            tail_index = this->max_delay - this->offset + this->current + i;
        } else {
            tail = this->current - this->offset + i;
        }
        pDst[i] = this->pData[tail_index];
    }
}

void DelayBlock::Feed(q15_t *pSrc, uint32_t block_size){
    for(uint32_t i = 0; i < block_size; i++){
        this->pData[this->current] = pSrc[i];
        if(++this->current >= this->max_delay){
            this->current = 0;
        }
    }
}
