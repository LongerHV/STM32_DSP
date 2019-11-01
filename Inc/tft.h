#ifndef __TFT_H
#define __TFT_H
#ifdef __cplusplus
extern "C"
{
#endif

// Includes
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

// Definitions
#define TFT_SOFTWARE_RESET ((uint8_t) 0x01)
#define TFT_PIXEL_FORMAT ((uint8_t) 0x3A)
#define TFT_COLUMN_ADDRESS_SET ((uint8_t) 0x2A)
#define TFT_ROW_ADDRESS_SET ((uint8_t) 0x2B)
#define TFT_MEMORY_WRITE ((uint8_t) 0x2C)

extern SPI_HandleTypeDef hspi1;

// Function prototypes
void TFT_CS_SET();
void TFT_CS_RESET();
void TFT_DC_SET();
void TFT_DC_RESET();
void TFT_Send(uint8_t data);
void TFT_SendCommand(uint8_t cmd);
void TFT_SendData(uint8_t data);
void TFT_SetColumn(uint8_t column);
void TFT_SetRow(uint8_t row);
void TFT_Init();
void TFT_DrawPixel(uint8_t row, uint8_t column, uint16_t colour);

#ifdef __cplusplus
}
#endif
#endif