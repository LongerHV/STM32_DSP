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

void Effect::SetName(const char *name){
    uint8_t i = 0;
    do{
        this->name[i] = name[i];
    } while(name[i++] != '\000');
}
