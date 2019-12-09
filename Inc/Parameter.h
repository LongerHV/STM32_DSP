#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "arm_math.h"
#include "main.h"

class Parameter {
   private:
    char name[11];
    char val_repr[5];
    char unit[3];
    uint8_t unit_length;
    float32_t value, min, max, step;
    float32_t *pValue;
    // int16_t value;
    friend class Effect;

   public:
    Parameter(const char *name, const char *unit, float32_t *pValue, float32_t min, float32_t max, float32_t step, float32_t initial);
    void SetName(const char *name);
    void SetUnit(const char *unit);
    void SetValue(float32_t value);
    float32_t GetValue();
    float32_t GetMin();
    float32_t GetMax();
    float32_t GetStep();
    char *GetName();
    char *GetValRepr();
    // int16_t GetValue();
    // int16_t *GetValuePtr();
    void UpdateValRepr();
    // virtual void UpdateValue() = 0;
    void UpdateValue();
};

// class Parameter_float32 : public Parameter {
//    private:
//     float32_t *pValue;
//     float32_t value;
//     float32_t min, max, step;

//    public:
//     Parameter_float32(const char *name, float32_t *pValue, float32_t min, float32_t max, float32_t initial);
//     ~Parameter_float32();
//     float32_t GetValue();
//     void SetValue(float32_t val);
//     void UpdateValue();
// };

// class Parameter_uint32 : public Parameter {
//    private:
//     uint32_t *pValue;
//     uint32_t value;
//     uint32_t min, max, step;

//    public:
//     Parameter_uint32(const char *name, uint32_t *pValue, uint32_t min, uint32_t max, uint32_t initial);
//     ~Parameter_uint32();
//     uint32_t GetValue();
//     void SetValue(uint32_t value);
//     void UpdateValue();
// };

#endif