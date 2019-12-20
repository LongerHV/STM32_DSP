/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
// #include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdlib.h>
#include "mymath.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint8_t block_ready;
extern uint16_t* input_buffer;
extern uint16_t* hidden_buffer;
extern uint16_t* output_buffer;
extern float32_t delay_buffer1[];
extern float32_t delay_buffer2[];
extern float32_t mod_buffer1[];
extern float32_t mod_buffer2[];
extern uint8_t character_buffer[];
extern uint32_t input_sample[];
// extern Display *pDisp;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define SPI3_CS_Pin GPIO_PIN_0
#define SPI3_CS_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define ENCODER_BUTTON_Pin GPIO_PIN_11
#define ENCODER_BUTTON_GPIO_Port GPIOD
#define SPI1_DC_Pin GPIO_PIN_8
#define SPI1_DC_GPIO_Port GPIOB
#define SPI1_CS_Pin GPIO_PIN_9
#define SPI1_CS_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define BLOCK_SIZE 512
#define SAMPLING_RATE 48000
#define NO_OF_CHANNELS 2
#define NO_OF_BUFFERS 4
#define ADC_RESOLUTION 16
#define DELAY_SIZE (48000 + BLOCK_SIZE) // 1s + block size
#define MODULATION_DELAY_SIZE (2 * 1440 + BLOCK_SIZE) // 30ms + block size


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
