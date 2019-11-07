#ifndef EFFECT_H
#define EFFECT_H

#include "arm_math.h"
#include "mymath.h"

class Effect{
    private:
        char name[9];

    public:
        Effect();
        ~Effect();
        virtual void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size)=0;
        char *GetName();
        void SetName(const char *name);

};

#endif