#ifndef EFFECT_H
#define EFFECT_H

#include "arm_math.h"
#include "mymath.h"

class Effect{
    private:
        char name[8];

    public:
        Effect();
        virtual void ProcessBlock(float32_t *pData, uint32_t block_size);
        char *GetName();

};

#endif