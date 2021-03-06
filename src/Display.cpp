#include "Display.h"
#include "characters.h"
#include "main.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_spi.h"

// Command definitions
#define NOP ((uint8_t)0x00)
#define SWRESET ((uint8_t)0x01)
#define RDDID ((uint8_t)0x04)
#define RDDST ((uint8_t)0x09)
#define RDDPM ((uint8_t)0x0A)
#define RDDMADCTL ((uint8_t)0x0B)
#define RDDCOLMOD ((uint8_t)0x0C)
#define RDDIM ((uint8_t)0x0D)
#define RDDSM ((uint8_t)0x0E)
#define SLPIN ((uint8_t)0x10)
#define SLPOUT ((uint8_t)0x11)
#define PTLON ((uint8_t)0x12)
#define NORON ((uint8_t)0x13)
#define INVOFF ((uint8_t)0x20)
#define INVON ((uint8_t)0x21)
#define GAMSET ((uint8_t)0x26)
#define DISPOFF ((uint8_t)0x28)
#define DISPON ((uint8_t)0x29)
#define CASET ((uint8_t)0x2A)
#define RASET ((uint8_t)0x2B)
#define RAMWR ((uint8_t)0x2C)
#define RAMRD ((uint8_t)0x2E)
#define PTLAR ((uint8_t)0x30)
#define TEOFF ((uint8_t)0x30)
#define TEON ((uint8_t)0x34)
#define MADCTL ((uint8_t)0x35)
#define IDMOFF ((uint8_t)0x36)
#define IDMON ((uint8_t)0x38)
#define COLMOD ((uint8_t)0x3A)
#define RDID1 ((uint8_t)0xDA)
#define RDID2 ((uint8_t)0xDB)
#define RDID3 ((uint8_t)0xDC)

// Pixel formats
#define PIX12BIT ((uint8_t)0x03)
#define PIX16BIT ((uint8_t)0x05)
#define PIX18BIT ((uint8_t)0x06)

// Char size
#define CHARSIZE ((uint8_t)0x08)
#define CHARSIZEBIG ((uint8_t)0x10)

Display::Display(uint8_t width, uint8_t height, SPI_HandleTypeDef *hspi, uint8_t *buffer) {
    this->width = width;
    this->height = height;
    this->hspi = hspi;
    this->pBuffer = buffer;
    this->pHead = NULL;
    this->pTail = NULL;

    // Init routine
    this->CS_RESET();
    HAL_Delay(20);
    this->SendCommand(SLPOUT);
    HAL_Delay(20);
    this->SendCommand(COLMOD);
    this->SendData(PIX16BIT);
    this->SendCommand(DISPON);
}

Display::~Display() {
}

void Display::CS_SET() {
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void Display::CS_RESET() {
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void Display::DC_SET() {
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_SET);
}

void Display::DC_RESET() {
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_RESET);
}

void Display::SetRow(uint16_t row) {
    this->SendCommand(CASET);
    this->SendData(row);
}

void Display::SetCol(uint16_t col) {
    this->SendCommand(RASET);
    this->SendData(col);
}

void Display::SetRect(uint16_t Ystart, uint16_t Xstart, uint16_t Ystop, uint16_t Xstop) {
    this->SendCommand(RASET);
    this->SendData(Ystart);
    this->SendData(Ystop);

    this->SendCommand(CASET);
    this->SendData(Xstart);
    this->SendData(Xstop);
}

void Display::Send(uint8_t data) {
    this->CS_RESET();
    HAL_SPI_Transmit(this->hspi, &data, 1, 5000);
    this->CS_SET();
}

void Display::Send(uint8_t *data, uint32_t length) {
    this->CS_RESET();
    HAL_SPI_Transmit(this->hspi, data, length, 5000);
    this->CS_SET();
}

void Display::SendCommand(uint8_t cmd) {
    this->DC_RESET();
    this->Send(cmd);
}

void Display::SendData(uint8_t data) {
    this->DC_SET();
    this->Send(data);
}

void Display::SendData(uint16_t data) {
    this->DC_SET();
    this->Send(data >> 8);
    this->Send(data);
}

void Display::SendData(uint8_t *data, uint32_t length) {
    this->DC_SET();
    this->Send(data, length);
}

void Display::SendDataDMA(uint8_t *data, uint32_t length) {
    this->DC_SET();
    this->CS_RESET();
    if (HAL_SPI_Transmit_DMA(this->hspi, this->pBuffer, 128) != HAL_OK) Error_Handler();
}

void Display::DrawPixel(uint8_t Y, uint8_t X, uint16_t colour) {
    this->SetRow(Y);
    this->SetCol(X);
    this->SendCommand(RAMWR);
    this->SendData(colour);
}

void Display::DrawChar(uint8_t Y, uint8_t X, char c, uint16_t colourF, uint16_t colourB) {
    uint8_t char_size = CHARSIZE;
    this->SetRect(Y * char_size, X * char_size, (Y + 1) * char_size - 1, (X + 1) * char_size - 1);
    this->SendCommand(RAMWR);
    for (uint8_t i = 0; i < char_size; i++) {
        for (uint8_t j = 0; j < char_size; j++) {
            if (characters[(uint8_t)c][i] & (0x01 << j)) {
                this->SendData(colourF);
            } else {
                this->SendData(colourB);
            }
        }
    }
}

void Display::UpdateChar(uint8_t Y, uint8_t X, char c, uint16_t colour) {
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            if (characters[(uint8_t)c][i] & (0x01 << (j / 2))) {
                pBuffer[16 * i + j] = colour >> 8;
                j++;
                pBuffer[16 * i + j] = colour;
            } else {
                pBuffer[16 * i + j] = 0x00;
                j++;
                pBuffer[16 * i + j] = 0x00;
            }
        }
    }

    SCB_CleanDCache_by_Addr((uint32_t *)pBuffer, 160);
    this->SetRect(8 * Y, 8 * X, 8 * Y + 7, 8 * X + 7);
    this->SendCommand(RAMWR);
    this->SendDataDMA(this->pBuffer, 128);
}

void Display::DrawRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop, uint16_t colour) {
    this->SetRect(Ystart, Xstart, Ystop - 1, Xstop - 1);
    this->SendCommand(RAMWR);
    for (uint16_t i = 0; i < ((Ystop - Ystart) * (Xstop - Xstart)); i++) {
        this->SendData(colour);
    }
}

void Display::FillScreen(uint16_t colour) {
    this->DrawRect(0x00, 0x00, this->height, this->width, colour);
}

void Display::PrintString(uint8_t Y, uint8_t X, const char *s) {
    for (uint8_t i = 0; s[i] != '\000'; i++) {
        this->DrawChar(Y, X + i, s[i], WHITE, BLACK);
    }
}

void Display::DumpASCII() {
    uint8_t index;
    for (uint8_t i = 0; i < 20; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            index = i * 16 + j;
            if (index >= 128) return;
            this->DrawChar(i, j, index, 0xFFFF, 0x0000);
        }
    }
}

void Display::Pop() {
    if (this->pHead == NULL) {
        return;
    } else if (this->pHead == this->pTail) {
        this->UpdateChar(this->pHead->Y, this->pHead->X, this->pHead->c, this->pHead->colour);
        delete pHead;
        this->pHead = this->pTail = NULL;
    } else {
        this->UpdateChar(this->pHead->Y, this->pHead->X, this->pHead->c, this->pHead->colour);
        node *pTemp = this->pHead;
        this->pHead = this->pHead->pNext;
        delete pTemp;
    }
}

void Display::PushChar(uint8_t Y, uint8_t X, char c, uint16_t colour) {
    node *pNew = new node{Y, X, c, colour, NULL};
    if (this->pHead == NULL && this->pTail == NULL) {
        this->pHead = this->pTail = pNew;
    } else if (this->pHead == this->pTail) {
        this->pHead->pNext = pNew;
        this->pTail = this->pHead->pNext;
    } else {
        this->pTail->pNext = pNew;
        this->pTail = this->pTail->pNext;
    }
}

void Display::PushString(uint8_t Y, uint8_t X, const char *s, uint16_t colour) {
    for (uint8_t i = 0; s[i] != '\000'; i++) {
        this->PushChar(Y, X + i, s[i], colour);
    }
}
