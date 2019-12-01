#ifndef EFFECT_H
#define EFFECT_H

#include "arm_math.h"
#include "mymath.h"
#include "Parameter.hpp"

class Effect{
    protected:
        char name[9];
        friend class MultiEffect;

    public:
        Parameter *parameters[10];
        uint8_t number_of_parameters;
        int8_t current_parameter;

        Effect();
        ~Effect();
        virtual void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size)=0;
        char *GetName();
        char *GetParamName(int8_t param_id);
        char *GetParamValRepr(int8_t param_id);
        Parameter *GetCurrentParam();
        void SetName(const char *name);

};

#endif