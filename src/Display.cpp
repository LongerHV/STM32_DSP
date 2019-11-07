#include "Display.hpp"
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"
#include "characters.h"

// Command definitions
#define NOP         ((uint8_t) 0x00)
#define SWRESET     ((uint8_t) 0x01)
#define RDDID       ((uint8_t) 0x04)
#define RDDST       ((uint8_t) 0x09)
#define RDDPM       ((uint8_t) 0x0A)
#define RDDMADCTL   ((uint8_t) 0x0B)
#define RDDCOLMOD   ((uint8_t) 0x0C)
#define RDDIM       ((uint8_t) 0x0D)
#define RDDSM       ((uint8_t) 0x0E)
#define SLPIN       ((uint8_t) 0x10)
#define SLPOUT      ((uint8_t) 0x11)
#define PTLON       ((uint8_t) 0x12)
#define NORON       ((uint8_t) 0x13)
#define INVOFF      ((uint8_t) 0x20)
#define INVON       ((uint8_t) 0x21)
#define GAMSET      ((uint8_t) 0x26)
#define DISPOFF     ((uint8_t) 0x28)
#define DISPON      ((uint8_t) 0x29)
#define CASET       ((uint8_t) 0x2A)
#define RASET       ((uint8_t) 0x2B)
#define RAMWR       ((uint8_t) 0x2C)
#define RAMRD       ((uint8_t) 0x2E)
#define PTLAR       ((uint8_t) 0x30)
#define TEOFF       ((uint8_t) 0x30)
#define TEON        ((uint8_t) 0x34)
#define MADCTL      ((uint8_t) 0x35)
#define IDMOFF      ((uint8_t) 0x36)
#define IDMON       ((uint8_t) 0x38)
#define COLMOD      ((uint8_t) 0x3A)
#define RDID1       ((uint8_t) 0xDA)
#define RDID2       ((uint8_t) 0xDB)
#define RDID3       ((uint8_t) 0xDC)

// Pixel formats
#define PIX12BIT    ((uint8_t) 0x03)
#define PIX16BIT    ((uint8_t) 0x05)
#define PIX18BIT    ((uint8_t) 0x06)


Display::Display(uint8_t width, uint8_t height, SPI_HandleTypeDef *hspi){
    this->width = width;
    this->height = height;
    this->hspi = hspi;
}

Display::~Display(){

}
