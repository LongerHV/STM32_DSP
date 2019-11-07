#include "Effect.hpp"

Effect::Effect(){

}

Effect::~Effect(){

}

char *Effect::GetName(){
    return &this->name[0];
}

void Effect::SetName(char name[9]){
    for(uint8_t i = 0; i < 8; i++){
        this->name[i] = name[i];
    }
}
