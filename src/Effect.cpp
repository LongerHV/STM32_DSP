#include "Effect.hpp"

Effect::Effect(){

}

Effect::~Effect(){

}

char *Effect::GetName(){
    return &this->name[0];
}

char *Effect::GetParamName(int8_t param_id){
    return this->parameters[param_id]->GetName();
}

char *Effect::GetParamValRepr(int8_t param_id){
    return this->parameters[param_id]->GetValRepr();
}

Parameter *Effect::GetCurrentParam(){
    return this->parameters[this->current_parameter];
}

void Effect::SetName(const char *name){
    uint8_t i;
    for(i = 0; name[i] != '\000'; i++){
        this->name[i] = name[i];
    }
    for(; i < 8; i++){
        this->name[i] = ' ';
    }
    this->name[8] = '\000';
}
