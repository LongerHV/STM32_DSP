#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "arm_math.h"

class Parameter {
   private:
    char name[11];
    int8_t value;
    char val_repr[5];
    friend class Effect;

   public:
    Parameter();
    void SetName(const char *name);
    void SetValue(uint8_t value);
    char *GetName();
    char *GetValRepr();
    int8_t GetValue();
    int8_t *GetValuePtr();
    void UpdateValRepr();
    virtual void UpdateValue() = 0;
};

class Parameter_float32 : public Parameter {
   private:
    float32_t *pValue;
    float32_t max_value;

   public:
    Parameter_float32(const char *name, float32_t *pValue, float32_t max_value);
    ~Parameter_float32();
    void UpdateValue();
};

class Parameter_uint32 : public Parameter {
   private:
    uint32_t *pValue;
    uint32_t max_value;

   public:
    Parameter_uint32(const char *name, uint32_t *pValue, uint32_t max_value);
    ~Parameter_uint32();
    void UpdateValue();
};

#endif