#include "Effect.hpp"

Effect::Effect(){

}

Effect::~Effect(){

}

char *Effect::GetName(){
    return &this->name[0];
}

void Effect::SetName(const char *name){
    for(uint8_t i = 0; i < 9; i++){
        this->name[i] = name[i];
    }
}
