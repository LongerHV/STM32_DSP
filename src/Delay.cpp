#include "Delay.hpp"

Delay::Delay(float32_t feedback, float32_t dry_level, float32_t wet_level){
    this->feedback = feedback;
    this->dry_level = dry_level;
    this->wet_level = wet_level;
}

void Delay::ProcessBlock(uint32_t block_size){

}
