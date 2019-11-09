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

// Char size
#define CHARSIZE    ((uint8_t) 0x08)
#define CHARSIZEBIG ((uint8_t) 0x10)



Display::Display(uint8_t width, uint8_t height, SPI_HandleTypeDef *hspi){
    this->width = width;
    this->height = height;
    this->hspi = hspi;

    // Init routine
    this->CS_RESET();
    HAL_Delay(20);
    this->SendCommand(SLPOUT);
    HAL_Delay(20);
    this->SendCommand(COLMOD);
    this->SendData(PIX16BIT);
    this->SendCommand(DISPON);
}

Display::~Display(){

}

void Display::CS_SET(){
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void Display::CS_RESET(){
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void Display::DC_SET(){
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_SET);
}

void Display::DC_RESET(){
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_RESET);
}

void Display::SetRow(uint8_t row){
    this->SendCommand(CASET);
    this->SendData(0x00);
    this->SendData(row);
}

void Display::SetCol(uint8_t col){
    this->SendCommand(RASET);
    this->SendData(0x00);
    this->SendData(col);
}

void Display::SetRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop){
    this->SendCommand(RASET);
    this->SendData(0x00);
    this->SendData(Ystart);
    this->SendData(0x00);
    this->SendData(Ystop);

    this->SendCommand(CASET);
    this->SendData(0x00);
    this->SendData(Xstart);
    this->SendData(0x00);
    this->SendData(Xstop);
}

void Display::Send(uint8_t data){
    this->CS_RESET();
    HAL_SPI_Transmit(this->hspi, &data, 1, 5000);
    this->CS_SET();
}

void Display::Send(uint8_t *data, uint32_t length){
    this->CS_RESET();
    HAL_SPI_Transmit(this->hspi, data, length, 5000);
    this->CS_SET();
}

void Display::SendCommand(uint8_t cmd){
    this->DC_RESET();
    this->Send(cmd);
}

void Display::SendData(uint8_t data){
    this->DC_SET();
    this->Send(data);
}

void Display::SendData(uint8_t *data, uint32_t length){
    this->DC_SET();
    this->Send(data, length);
}

void Display::DrawPixel(uint8_t Y, uint8_t X, uint16_t colour){
    this->SetRow(Y);
    this->SetCol(X);
    this->SendCommand(RAMWR);
    this->SendData(colour >> 8);
    this->SendData(colour);
}

void Display::DrawChar(uint8_t Y, uint8_t X, char c, uint16_t colourF, uint16_t colourB){
    uint8_t char_size = CHARSIZE;
    this->SetRect(Y * char_size, X * char_size, (Y + 1) * char_size - 1, (X + 1) * char_size - 1);
    this->SendCommand(RAMWR);
    for(uint8_t i = 0; i < char_size; i++){
        for(uint8_t j = 0; j < char_size; j++){
            if(characters[(uint8_t) c][i] & (0x01 << j)){
                this->SendData(colourF >> 8);
                this->SendData(colourF);
            } else{
                this->SendData(colourB >> 8);
                this->SendData(colourB);
            }
        }
    }
}

void Display::DrawRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop, uint16_t colour){
    this->SetRect(Ystart, Xstart, Ystop, Xstop);
    this->SendCommand(RAMWR);
    for(uint16_t i = 0; i < ((Ystop - Ystart) * (Xstop - Xstart)); i++){
        this->SendData(colour >> 8);
        this->SendData(colour);
    }
}

void Display::FillScreen(uint16_t colour){
    this->DrawRect(0x00, 0x00, this->height - 1, this-> width - 1, colour);
}

void Display::DumpASCII(){
    uint8_t index;
    for(uint8_t i = 0; i < 20; i++){
        for(uint8_t j = 0; j < 16; j++){
            index = i * 16 + j;
            if(index >= 128) return;
            this->DrawChar(i, j, index, 0xFFFF, 0x0000);
        }
    }
}
