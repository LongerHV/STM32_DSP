#include "Parameter.h"
#include "stdio.h"

Parameter::Parameter(const char *name, const char *unit, float32_t *pValue, float32_t min, float32_t max, float32_t step, float32_t initial) {
    this->SetName(name);
    this->SetUnit(unit);
    // this->pValue = pValue;
    this->min = min;
    this->max = max;
    this->step = step;
    this->value = initial;
    // this->SetValue(initial);
    // this->UpdateValue();
    this->UpdateValRepr();
}

void Parameter::SetName(const char *name){
    uint8_t i;
    for (i = 0; name[i] != '\000' && i < 10; i++) {
        this->name[i] = name[i];
    }
    for (; i < 10; i++) {
        this->name[i] = ' ';
    }
    this->name[10] = '\000';
}

void Parameter::SetUnit(const char *unit) {
    this->unit_length = 0xFF;
    for(uint8_t i = 0; i < 3; i++) {
        this->unit[i] = unit[i];
        if(this->unit_length == 0xFF && unit[i] == '\000')
            this->unit_length = i;
    }
}

char *Parameter::GetName() {
    return &this->name[0];
}

char *Parameter::GetValRepr() {
    return &this->val_repr[0];
}

float32_t Parameter::GetValue() {
    return this->value;
}

float32_t Parameter::GetMin() {
    return this->min;
}

float32_t Parameter::GetMax() {
    return this->max;
}

float32_t Parameter::GetStep() {
    return this->step;
}

void Parameter::SetValue(float32_t value) {
    this->value = value;
}

// int8_t *Parameter::GetValuePtr() {
//     return &this->value;
// }

void Parameter::UpdateValRepr() {
    uint8_t repr_length = 4 - this->unit_length;
    uint8_t point;
    char temp[64];
    if (this->unit[0] == '%' || (this->unit[0] == 'm' && this->unit[1] == 's' || this->unit[0] == ' '))
        point = 0;
    else
        point = 1;
    
    // this->val_repr[4] = '\000';
    if(point){
        int a = (int) this->value;
        int b = (int)((this->value - (float32_t)a) * 100);
        if(a)
            snprintf(this->val_repr, 5, "%d.%d%s  ", a, b, this->unit);
        else
            snprintf(this->val_repr, 5, ".%d%s  ", b, this->unit);

        
    } else {
        snprintf(this->val_repr, 5, "%d%s  ", (int)this->value, this->unit);
    }

    // for(uint8_t i = 0; i < 4 - repr_length; i++){
    //     this->val_repr[repr_length + i] = this->unit[i];
    // }
        
    // } else if(this->unit[0] == '%'){
    //     if (this->value < 10) {
    //         this->val_repr[0] = ' ';
    //         this->val_repr[1] = ' ';
    //         this->val_repr[2] = (char)(((uint8_t)this->value % 10) + 48);
    //         this->val_repr[3] = '%';
    //         this->val_repr[4] = '\000';
    //     } else if (this->value < 100) {
    //         this->val_repr[0] = ' ';
    //         this->val_repr[1] = (char)(((uint8_t)this->value % 100) / 10 + 48);
    //         this->val_repr[2] = (char)(((uint8_t)this->value % 10) + 48);
    //         this->val_repr[3] = '%';
    //         this->val_repr[4] = '\000';
    //     } else if (this->value == 100) {
    //         this->val_repr[0] = '1';
    //         this->val_repr[1] = '0';
    //         this->val_repr[2] = '0';
    //         this->val_repr[3] = '%';
    //         this->val_repr[4] = '\000';
    //     }
    // }

}

// Parameter_float32::Parameter_float32(const char *name, const char *unit, float32_t *pValue, float32_t min, float32_t max, float32_t initial) {
//     this->SetName(name);
//     this->SetUnit(unit);
//     this->pValue = pValue;
//     this->min = min;
//     this->max = max;
//     this->value = initial;
//     // this->SetValue(initial);
//     this->UpdateValue();
// }

// Parameter_float32::~Parameter_float32(){
// }

// float32_t Parameter_float32::GetValue() {
//     return this->value;
// }

// void Parameter_float32::SetValue(float32_t val) {
//     this->value = val;
// }

// void Parameter_float32::UpdateValue(){
//     *this->pValue = (float32_t)this->GetValue() * 0.01 * this->max_value; 
// }

// Parameter_uint32::Parameter_uint32(const char *name, const char *unit, uint32_t *pValue, uint32_t min, uint32_t max, uint32_t initial) {
//     this->SetName(name);
//     this->SetUnit(unit);
//     this->pValue = pValue;
//     this->min = min;
//     this->max = max;
//     this->value = initial;
//     // this->SetValue(initial);
//     this->UpdateValue();
// }

// Parameter_uint32::~Parameter_uint32(){

// }

// void Parameter_uint32::UpdateValue(){
//     *this->pValue = (uint32_t)((float32_t)this->GetValue() * 0.01 * (float32_t)this->max_value); 
// }
