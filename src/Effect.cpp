#include "Effect.hpp"

Effect::Effect(){

}

char *Effect::GetName(){
    return &this->name[0];
}
