#ifndef EFFECT_H
#define EFFECT_H

#include "arm_math.h"
#include "mymath.h"
#include "Parameter.hpp"

class Effect{
    protected:
        char name[9];

    public:
        Parameter *parameters[10];
        Effect();
        ~Effect();
        virtual void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size)=0;
        char *GetName();
        char *GetParamName(int8_t param_id);
        char *GetParamValRepr(int8_t param_id);
        void SetName(const char *name);

};

#endif