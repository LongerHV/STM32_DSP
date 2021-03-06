#ifndef MULTIEFFECT_H
#define MULTIEFFECT_H

#include "Display.h"
#include "Effect.h"
#include "VU.h"
#include "arm_math.h"
#include "main.h"

class MultiEffect {
   private:
    Display *my_disp;
    Effect *current_effect, *pHead, *pTail;
    VU *VU_pre_L, *VU_pre_R, *VU_post_L, *VU_post_R;
    TIM_HandleTypeDef *htim;
    uint8_t param_flag = 1;
    uint8_t val_flag = 0;
    uint8_t button_states = 0;
    uint8_t button_pressed = 0;
    uint16_t last_tim_cnt = 0x7FFF;

    void InitializeLCD(SPI_HandleTypeDef *hspi);
    void InitializeEffects();

   public:
    MultiEffect(TIM_HandleTypeDef *encoder_timer, SPI_HandleTypeDef *tft_spi);
    void ProcessBlock(float32_t *pData_left, float32_t *pData_right, uint32_t block_size);
    void UpdateUI();
    float32_t UpdateEncoder(TIM_HandleTypeDef *htim, float32_t var, float32_t min, float32_t max, float32_t step);
    uint8_t UpdateEncoder(TIM_HandleTypeDef *htim);
    void DisplayPop();
    void AddEffect(Effect *pNew);


};

#endif