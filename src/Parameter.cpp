#include "Parameter.hpp"

Parameter::Parameter(){

}

char* Parameter::GetName(){
    return *this->name;
}

uint8_t Parameter::GetValue(){
    return this->value;
}

void Parameter::IncrementValue(){
    if(this->value >= 100)
        return
    this->value++;
}

void Parameter::DecrementValue(){
    if(this->value == 0)
        return
    this->value--;

}