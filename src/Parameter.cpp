#include "Parameter.hpp"

Parameter::Parameter(const char *name){
    for(uint8_t i = 0; i < 11; i++){
        this->name[i] = name[i];
    }
}

char* Parameter::GetName(){
    return &this->name[0];
}

uint8_t Parameter::GetValue(){
    return this->value;
}

void Parameter::IncrementValue(){
    if(this->value >= 100){
        return;
    }
    this->value++;
}

void Parameter::DecrementValue(){
    if(this->value == 0){
        return;
    }
    this->value--;

}