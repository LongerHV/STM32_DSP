#include "Parameter.hpp"

Parameter::Parameter(const char *name){
    for(uint8_t i = 0; i < 11; i++){
        this->name[i] = name[i];
    }
    this->value = 0;
    this->UpdateValRepr();
}

char* Parameter::GetName(){
    return &this->name[0];
}

char *Parameter::GetValRepr(){
    return &this->val_repr[0];
}

int8_t Parameter::GetValue(){
    return this->value;
}

int8_t *Parameter::GetValuePtr(){
    return &this->value;
}

void Parameter::IncrementValue(){
    if(this->value >= 100){
        return;
    }
    this->value++;
    this->UpdateValRepr();
}

void Parameter::DecrementValue(){
    if(this->value == 0){
        return;
    }
    this->value--;
    this->UpdateValRepr();
}

void Parameter::UpdateValRepr(){
    if(this->value < 10){
        this->val_repr[0] = ' ';
        this->val_repr[1] = ' ';
        this->val_repr[2] = (char)((this->value % 10) + 48);
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    }else if(this->value < 100){
        this->val_repr[0] = ' ';
        this->val_repr[1] = (char)((this->value % 100) / 10 + 48);
        this->val_repr[2] = (char)((this->value % 10) + 48);
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    }else if(this->value == 100){
        this->val_repr[0] = '1';
        this->val_repr[1] = '0';
        this->val_repr[2] = '0';
        this->val_repr[3] = '%';
        this->val_repr[4] = '\000';
    }
}