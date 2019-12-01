#include "MultiEffect.hpp"
#include "DelayBlock.hpp"
#include "Delay.hpp"
#include "main.h"


MultiEffect::MultiEffect(TIM_HandleTypeDef *encoder_timer, SPI_HandleTypeDef *tft_spi){
    this->htim = encoder_timer;

    this->InitializeLCD(tft_spi);
    this->InitializeEffects();

    // printing UI
    this->my_disp->PushString(0, 0, this->current_effect->GetName(), WHITE);
    for(int8_t i = 0; i < 5; i++){
        this->my_disp->PushString(4 + i, 1, this->current_effect->GetParamName(i), WHITE);
        this->my_disp->PushChar(4 + i, 11, ':', WHITE);
        this->my_disp->PushString(4 + i, 12, this->current_effect->GetParamValRepr(i), WHITE);
    }
    this->my_disp->PushChar(4, 0, '>', WHITE);
}

void MultiEffect::InitializeLCD(SPI_HandleTypeDef *hspi){
    this->my_disp = new Display(128, 160, hspi, &character_buffer[0]);
    my_disp->FillScreen(BLACK);
}

void MultiEffect::InitializeEffects(){
    DelayBlock *left_delay = new DelayBlock(&delay_buffer2[0], 48000, 40000);
    DelayBlock *right_delay = new DelayBlock(&delay_buffer1[0], 48000, 35000);
    this->current_effect = new Delay("Delay", left_delay, right_delay, 0.6, 1.0, 0.8);
}

void MultiEffect::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size){
    this->current_effect->ProcessBlock(pData_left, pData_right, block_size);
}

void MultiEffect::UpdateUI(){
    // Button press detection
    button_states <<= 1;
    button_states |= !HAL_GPIO_ReadPin(ENCODER_BUTTON_GPIO_Port, ENCODER_BUTTON_Pin);
    button_pressed = button_states == 1;
    if(button_pressed){
        uint16_t colour;
        if(param_flag){
        colour = GREEN; 
        }else{
        colour = WHITE;
        }
        my_disp->PushChar(4 + current_effect->current_parameter, 0, '>', colour);
        my_disp->PushString(4 + current_effect->current_parameter, 1, current_effect->GetCurrentParam()->GetName(), colour);
        my_disp->PushChar(4 + current_effect->current_parameter, 11, ':', colour);
        my_disp->PushString(4 + current_effect->current_parameter, 12, current_effect->GetCurrentParam()->GetValRepr(), colour);

        val_flag = param_flag;
        param_flag = !param_flag;
        button_pressed = 0;
    }

    // Change effect
    if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)){
        if(false){
        // if(this->UpdateEncoder(this->htim, &cnt_effect, 0, 1)){
        my_disp->PushString(0, 0, current_effect->GetName(), WHITE);
        for(int8_t i = 0; i < 5; i++){
            my_disp->PushString(4 + i, 1, current_effect->GetParamName(i), WHITE);
            my_disp->PushChar(4 + i, 11, ':', WHITE);
            my_disp->PushString(4 + i, 12, current_effect->GetParamValRepr(i), WHITE);
        }
        my_disp->PushChar(4 + current_effect->current_parameter, 0, ' ', WHITE);
        my_disp->PushChar(4, 0, '>', WHITE);
        current_effect->current_parameter = 0;
        param_flag = 1;
        val_flag = 0;
        }
    // Change parameter
    }else if(param_flag){
        if(this->UpdateEncoder(this->htim, &current_effect->current_parameter, 0, 4)){
        for(uint8_t i = 0; i < current_effect->number_of_parameters; i++){
            if(i == current_effect->current_parameter){
            my_disp->PushChar(4 + i, 0, '>', WHITE);
            }else{
            my_disp->PushChar(4 + i, 0, ' ', WHITE);
            }
        }

        }
    // Modify parameter value
    }else if(val_flag){
        if(this->UpdateEncoder(this->htim, current_effect->GetCurrentParam()->GetValuePtr(), 0, 100)){
        current_effect->GetCurrentParam()->UpdateValRepr();
        my_disp->PushString(4 + current_effect->current_parameter, 12, current_effect->GetCurrentParam()->GetValRepr(), GREEN);
        }
    }

    this->my_disp->Pop();

}

uint8_t MultiEffect::UpdateEncoder(TIM_HandleTypeDef *htim, int8_t *var, uint8_t min, uint8_t max){
  static uint16_t last_tim_cnt = 0;
  int tim_diff = htim->Instance->CNT - last_tim_cnt;
  uint8_t ret_val = 0;
  if(tim_diff >= 4 || tim_diff <= -4){
    ret_val = 1;
    tim_diff /= 4;
    *var -= (int8_t)tim_diff;
    if(*var > max){
      *var = max;
      ret_val = 0;
    } 
    if(*var < min) {
      *var = min;
      ret_val = 0;
    }
    last_tim_cnt = htim->Instance->CNT;
  }
  return ret_val;
}