#include "Effect.hpp"

Effect::Effect(){

}

Effect::~Effect(){

}

char *Effect::GetName(){
    return &this->name[0];
}

void Effect::SetName(const char *name){
    uint8_t i = 0;
    do{
        this->name[i] = name[i];
    } while(name[i++] != '\000');
}
