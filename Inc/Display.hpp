#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

class Display{
    private:
        uint8_t width;
        uint8_t height;
    public:
        Display(uint8_t width, uint8_t height);
        ~Display();
};

#endif