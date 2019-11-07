#ifndef DISPLAY_H
#define DISPLAY_H

#include "arm_math.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"

class Display{
    private:
        uint8_t width;
        uint8_t height;

        SPI_HandleTypeDef *hspi;

        void CS_SET();
        void CS_RESET();
        void DC_SET();
        void DC_RESET();
        void SetRow(uint8_t row);
        void SetCol(uint8_t col);
        void SetRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop);

    public:
        Display(uint8_t width, uint8_t height, SPI_HandleTypeDef *hspi);
        ~Display();
        void Send(uint8_t data);
        void Send(uint8_t *data, uint32_t length);
        void SendCommand(uint8_t cmd);
        void SendData(uint8_t data);
        void SendData(uint8_t *data, uint32_t length);
        void DrawPixel(uint8_t Y, uint8_t X, uint16_t colour);
        void DrawChar(uint8_t Y, uint8_t X, char c, uint16_t colourF, uint16_t colourB, uint8_t size);
        void DrawRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop, uint16_t colour);
        void FillScreen(uint16_t colour);
        void DumpASCII();
};

#endif