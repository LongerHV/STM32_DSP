#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "arm_math.h"

class Parameter{
    private:
        char name[11];
        uint8_t value;
        friend class Effect;

    public:
        Parameter(const char *name);
        char* GetName();
        uint8_t GetValue();
        void IncrementValue();
        void DecrementValue();

};


#endif