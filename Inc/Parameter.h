#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "arm_math.h"
#include "main.h"

class Parameter {
   private:
    char name[10];
    char val_repr[6];
    char unit[3];
    int32_t value, min, max, step;
    friend class Effect;

   public:
    Parameter(const char *name, const char *unit, int32_t min, int32_t max, int32_t step, int32_t initial);
    void SetName(const char *name);
    void SetUnit(const char *unit);
    void SetValue(int32_t value);
    int32_t GetValue();
    int32_t GetMin();
    int32_t GetMax();
    int32_t GetStep();
    char *GetName();
    char *GetValRepr();
    void UpdateValRepr();
    void UpdateValue();
};

#endif