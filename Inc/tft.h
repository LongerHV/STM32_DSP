#ifndef __TFT_H
#define __TFT_H
#ifdef __cplusplus
extern "C"
{
#endif

// Includes
#include "main.h"
// #include "characters.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

// Definitions
#define TFT_SOFTWARE_RESET ((uint8_t) 0x01)
#define TFT_PIXEL_FORMAT ((uint8_t) 0x3A)
#define TFT_CASET ((uint8_t) 0x2A)
#define TFT_RASET ((uint8_t) 0x2B)
#define TFT_RAMWR ((uint8_t) 0x2C)

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
void TFT_SetRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop);
void TFT_Init();
void TFT_DrawPixel(uint8_t Y, uint8_t X, uint16_t colour);
void TFT_DrawChar(uint8_t Y, uint8_t X, char c, uint16_t colourF, uint16_t colourB);
void TFT_DrawRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop, uint16_t colour);

#ifdef __cplusplus
}
#endif
#endif