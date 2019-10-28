include "fx.h"

// functions implementation
void FX_DELAY(FX_DelayTypeDef *delay, float32_t *pSource, float32_t *pDest, uint16_t size){
    uint16_t tail;
    for(uint16_t i = 0; i < size; i++){
        // Calculate index of delay tail
        if(delay.Offset > delay.Index){
            tail = delay.MaxSize - delay.Offset + delay.Index;
        } else {
            tail = delay.Index - delay.Offset;
        }

        // Calculate value for output sample
        pDest[i] = pSource[i] + (delay.Wet * delay.data[tail]);

        // Calculate value for delay input sample
        delay.data[index] = pSource[i] + (delay.Feedback * delay.data[delay.Index]);

        // Increment delay counter
        if(++delay.Index >= MaxSize){
            delay.Index = 0;
        }
    }
}


