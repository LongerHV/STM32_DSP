#include "Parameter.h"

Parameter::Parameter() {
}

Parameter::Parameter(const char *name) {
    this->SetName(name);
}

void Parameter::SetName(const char *name){
    uint8_t i;
    for (i = 0; name[i] != '\000' && i < 11; i++) {
        this->name[i] = name[i];
    }
    for (; name[i] < 10; i++) {
        this->name[i] = ' ';
    }
    this->name[10] = '\000';
    this->value = 50;
    this->UpdateValRepr();
}

char *Parameter::GetName() {
    return &this->name[0];
}

char *Parameter::GetValRepr() {
    return &this->val_repr[0];
}

int8_t Parameter::GetValue() {
    return this->value;
}

int8_t *Parameter::GetValuePtr() {
    return &this->value;
}

void Parameter::UpdateValRepr() {
    if (this->value < 10) {
        this->val_repr[0] = ' ';
        this->val_repr[1] = ' ';
        this->val_repr[2] = (char)((this->value % 10) + 48);
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    } else if (this->value < 100) {
        this->val_repr[0] = ' ';
        this->val_repr[1] = (char)((this->value % 100) / 10 + 48);
        this->val_repr[2] = (char)((this->value % 10) + 48);
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    } else if (this->value == 100) {
        this->val_repr[0] = '1';
        this->val_repr[1] = '0';
        this->val_repr[2] = '0';
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    }
}

Parameter_float32::Parameter_float32(const char *name, float32_t *pValue, float32_t max_value) {
    this->SetName(name);
    this->pValue = pValue;
    this->max_value = max_value;
    this->UpdateValue();
}

Parameter_float32::~Parameter_float32(){
}

void Parameter_float32::UpdateValue(){
    *this->pValue = (float32_t)this->GetValue() * 0.01 * this->max_value; 
}

Parameter_uint32::Parameter_uint32(const char *name, uint32_t *pValue, uint32_t max_value) {
    this->SetName(name);
    this->pValue = pValue;
    this->max_value = max_value;
    this->UpdateValue();
}

Parameter_uint32::~Parameter_uint32(){

}

void Parameter_uint32::UpdateValue(){
    *this->pValue = (uint32_t)((float32_t)this->GetValue() * 0.01 * (float32_t)this->max_value); 
}
