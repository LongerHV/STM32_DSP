#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "arm_math.h"

class Parameter{
    private:
        char name[11];
        int8_t value;
        char val_repr[5];
        friend class Effect;


    public:
        Parameter(const char *name);
        char* GetName();
        char *GetValRepr();
        int8_t GetValue();
        int8_t *GetValuePtr();
        void IncrementValue();
        void DecrementValue();
        void UpdateValRepr();
};


#endif