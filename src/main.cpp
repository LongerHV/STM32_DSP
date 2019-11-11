/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Delay.hpp"
#include "Display.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t block_counter = 0;
uint8_t block_ready = 0;
uint16_t buffer[6][BLOCK_SIZE];
// remember to specify in STM32H743ZI_FLASH.LD linker file
uint32_t __attribute__((section(".ahb_sram_d2"))) input_sample[2];
uint8_t __attribute__((section(".ahb_sram_d2"))) character_buffer[128];
q15_t __attribute__((section(".axi_sram_d1"))) delay_buffer1[48000];
q15_t __attribute__((section(".axi_sram_d1"))) delay_buffer2[48000];
// uint32_t __attribute__((section(".sdram"))) sdram_test[24000];
uint16_t *input_buffer;
uint16_t *hidden_buffer;
uint16_t *output_buffer;
uint16_t *temp_buffer;
float data[2][BLOCK_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC3_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  MX_FMC_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

    input_buffer = buffer[0];
    hidden_buffer = buffer[2];
    output_buffer = buffer[4];

    // Starting peripherials
    if (HAL_ADC_Start_DMA(&hadc3, input_sample, 2) != HAL_OK)
        return 0;
    if (HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1) != HAL_OK)
        return 0;
    if (HAL_DAC_Start(&hdac1, DAC1_CHANNEL_2) != HAL_OK)
        return 0;
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
        return 0;


    // // TESTING FMC
    // uint32_t *pSdram = (uint32_t *) 0xD0000000;
    // uint8_t Src[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    // for(uint32_t i = 0; i < 10; i++){
    //   sdram_test[i] = Src[i];
    //   // *(__IO uint32_t *) (pSdram + i) = Src[i];
    //   // HAL_SDRAM_Write_8b(&hsdram1, pSdram, Src, 10);
    // }
    // uint32_t Dst[10];
    // for(uint32_t i = 0; i < 10; i++){
    //   // Dst[i] = *(__IO uint32_t *) (0xD0000000 + 4 * i);
    //   // HAL_SDRAM_Read_32b(&hsdram1, pSdram, Dst, 10);
    // }

    // TESTING LCD SPI
    Display *my_disp = new Display(128, 160, &hspi1, &character_buffer[0]);
    my_disp->FillScreen(BLACK);
    // HAL_Delay(200);
    // my_disp->FillScreen(RED);
    // HAL_Delay(200);
    // my_disp->FillScreen(GREEN);
    // HAL_Delay(200);
    // my_disp->FillScreen(BLUE);
    // HAL_Delay(200);
    // my_disp->FillScreen(BLUE | GREEN);
    // HAL_Delay(200);
    // my_disp->FillScreen(BLUE | RED);
    // HAL_Delay(200);
    // my_disp->FillScreen(RED | GREEN);
    // HAL_Delay(200);
    // my_disp->FillScreen(BLACK);
    // HAL_Delay(200);

    // my_disp->DumpASCII();

    my_disp->UpdateChar('A');

    // Initializing effects
    DelayBlock *left_delay = new DelayBlock(&delay_buffer1[0], 48000, 40000);
    DelayBlock *right_delay = new DelayBlock(&delay_buffer2[0], 48000, 35000);
    Effect *delay1 = new Delay("Delay   ", left_delay, right_delay, 0.6, 1.0, 0.8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        if (block_ready)
        {
            // Convert data to float
            arm_uint16_to_float(&hidden_buffer[0], data[0], 2 * BLOCK_SIZE);
            // CODE HERE (modify data)

            // fx_delaySrereo(&fxdelayStereo, data[0], data[1], BLOCK_SIZE);
            delay1->ProcessBlock(data[0], data[1], BLOCK_SIZE);

            // CODE ENDS HERE
            // Convert data back to 16 bit unsigned integer
            arm_float_to_uint16(data[0], &hidden_buffer[0], 2 * BLOCK_SIZE);
            block_ready = 0;
        }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 99;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI3|RCC_PERIPHCLK_SPI1
                              |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_FMC;
  PeriphClkInitStruct.PLL2.PLL2M = 1;
  PeriphClkInitStruct.PLL2.PLL2N = 50;
  PeriphClkInitStruct.PLL2.PLL2P = 1;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
