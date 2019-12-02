#include "MultiEffect.h"
#include "Delay.h"
#include "DelayBlock.h"
#include "Modulation.h"
#include "main.h"

MultiEffect::MultiEffect(TIM_HandleTypeDef *encoder_timer, SPI_HandleTypeDef *tft_spi) {
    this->htim = encoder_timer;
    encoder_timer->Instance->CNT = 0x7FFF;

    this->InitializeLCD(tft_spi);

    this->current_effect = this->pHead = this->pTail = NULL;
    this->InitializeEffects();

    // printing UI
    this->my_disp->PushString(0, 0, this->current_effect->GetName(), WHITE);
    for (int8_t i = 0; i < this->current_effect->number_of_parameters; i++) {
        this->my_disp->PushString(4 + i, 1, this->current_effect->GetParamName(i), WHITE);
        this->my_disp->PushChar(4 + i, 11, ':', WHITE);
        this->my_disp->PushString(4 + i, 12, this->current_effect->GetParamValRepr(i), WHITE);
    }
    this->my_disp->PushChar(4, 0, '>', WHITE);

    this->VU_pre_L = new VU(19, 0, this->my_disp);
    this->VU_pre_R = new VU(19, 1, this->my_disp);
    this->VU_post_L = new VU(19, 14, this->my_disp);
    this->VU_post_R = new VU(19, 15, this->my_disp);
}

void MultiEffect::InitializeLCD(SPI_HandleTypeDef *hspi) {
    this->my_disp = new Display(128, 160, hspi, &character_buffer[0]);
    my_disp->FillScreen(BLACK);
}

void MultiEffect::InitializeEffects() {
    DelayBlock *left_delay = new DelayBlock(&delay_buffer2[0], 48000, 40000);
    DelayBlock *right_delay = new DelayBlock(&delay_buffer1[0], 48000, 35000);
    DelayBlock *left_mod_delay = new DelayBlock(&mod_buffer1[0], 4800, 480);
    DelayBlock *right_mod_delay = new DelayBlock(&mod_buffer2[0], 4800, 480);
    this->pHead = new Delay("Delay", left_delay, right_delay, 0.6, 1.0, 0.8);
    this->pTail = new Modulation("Mod", left_mod_delay, right_mod_delay, 0.75, 0.75, 0.0, 0.5, 1.0, 480);
    // this->pTail = new Delay("Delay 2", left_delay, right_delay, 0.0, 1.0, 0.0);
    this->pHead->pNext = this->pTail;
    this->pTail->pPrev = this->pHead;
    this->pHead->pPrev = NULL;
    this->pTail->pNext = NULL;
    this->current_effect = this->pHead;
}

void MultiEffect::ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size) {
    this->VU_pre_L->Update(pData_left, block_size);
    this->VU_pre_R->Update(pData_right, block_size);
    this->current_effect->ProcessBlock(pData_left, pData_right, block_size);
    this->VU_post_L->Update(pData_left, block_size);
    this->VU_post_R->Update(pData_right, block_size);
}

void MultiEffect::UpdateUI() {
    // Button press detection
    this->button_states <<= 1;
    this->button_states |= !HAL_GPIO_ReadPin(ENCODER_BUTTON_GPIO_Port, ENCODER_BUTTON_Pin);
    this->button_pressed = this->button_states == 1;
    if (this->button_pressed) {
        uint16_t colour;
        if (this->param_flag) {
            colour = GREEN;
        } else {
            colour = WHITE;
        }
        my_disp->PushChar(4 + current_effect->current_parameter, 0, '>', colour);
        my_disp->PushString(4 + current_effect->current_parameter, 1, current_effect->GetCurrentParam()->GetName(), colour);
        my_disp->PushChar(4 + current_effect->current_parameter, 11, ':', colour);
        my_disp->PushString(4 + current_effect->current_parameter, 12, current_effect->GetCurrentParam()->GetValRepr(), colour);

        this->val_flag = this->param_flag;
        this->param_flag = !this->param_flag;
        this->button_pressed = 0;
    }

    // Change effect
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
        if (this->UpdateEncoder(this->htim)) {
            uint8_t i;
            my_disp->PushString(0, 0, this->current_effect->GetName(), WHITE);
            for (i = 0; i < this->current_effect->number_of_parameters; i++) {
                this->my_disp->PushString(4 + i, 1, this->current_effect->GetParamName(i), WHITE);
                this->my_disp->PushChar(4 + i, 11, ':', WHITE);
                this->my_disp->PushString(4 + i, 12, this->current_effect->GetParamValRepr(i), WHITE);
                if (i == this->current_effect->current_parameter) {
                    this->my_disp->PushChar(4 + i, 0, '>', WHITE);
                } else {
                    this->my_disp->PushChar(4 + i, 0, ' ', WHITE);
                }
            }
            for (; i < 10; i++) {
                this->my_disp->PushString(4 + i, 0, "                ", WHITE);
            }
            this->param_flag = 1;
            this->val_flag = 0;
        }
        // Change parameter
    } else if (this->param_flag) {
        if (this->UpdateEncoder(this->htim, &this->current_effect->current_parameter, 0, this->current_effect->number_of_parameters - 1)) {
            for (uint8_t i = 0; i < this->current_effect->number_of_parameters; i++) {
                if (i == this->current_effect->current_parameter) {
                    this->my_disp->PushChar(4 + i, 0, '>', WHITE);
                } else {
                    this->my_disp->PushChar(4 + i, 0, ' ', WHITE);
                }
            }
        }
        // Modify parameter value
    } else if (this->val_flag) {
        if (this->UpdateEncoder(this->htim, this->current_effect->GetCurrentParam()->GetValuePtr(), 0, 100)) {
            this->current_effect->GetCurrentParam()->UpdateValRepr();
            this->my_disp->PushString(4 + this->current_effect->current_parameter, 12, this->current_effect->GetCurrentParam()->GetValRepr(), GREEN);
        }
    }

    this->my_disp->Pop();
}

uint8_t MultiEffect::UpdateEncoder(TIM_HandleTypeDef *htim, int8_t *var, uint8_t min, uint8_t max) {
    uint16_t current = htim->Instance->CNT;
    int tim_diff = current - this->last_tim_cnt;
    uint8_t ret_val = 0;
    if (tim_diff >= 4 || tim_diff <= -4) {
        ret_val = 1;
        tim_diff /= 4;
        *var -= (int8_t)tim_diff;
        if (*var > max) {
            *var = max;
            ret_val = 0;
        }
        if (*var < min) {
            *var = min;
            ret_val = 0;
        }
        this->last_tim_cnt = current;
    }
    return ret_val;
}

uint8_t MultiEffect::UpdateEncoder(TIM_HandleTypeDef *htim) {
    uint16_t current = htim->Instance->CNT;
    int tim_diff = current - this->last_tim_cnt;
    uint8_t ret_val = 0;
    if (tim_diff >= 4 || tim_diff <= -4) {
        ret_val = 1;
        tim_diff /= 4;
        if (tim_diff <= -1 && this->current_effect->pNext != NULL) {
            this->current_effect = this->current_effect->pNext;
        } else if (tim_diff >= 1 && this->current_effect->pPrev != NULL) {
            this->current_effect = this->current_effect->pPrev;
        } else {
            ret_val = 0;
        }
        this->last_tim_cnt = current;
    }
    return ret_val;
}