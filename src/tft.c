#include "tft.h"
#include "characters.h"

// Functions implementations
void TFT_CS_SET(){
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void TFT_CS_RESET(){
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void TFT_DC_SET(){
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_SET);
}

void TFT_DC_RESET(){
    HAL_GPIO_WritePin(SPI1_DC_GPIO_Port, SPI1_DC_Pin, GPIO_PIN_RESET);
}

void TFT_Send(uint8_t data){
    TFT_CS_RESET();
    HAL_SPI_Transmit(&hspi1, &data, 1, 5000);
    TFT_CS_SET();
}

void TFT_SendCommand(uint8_t cmd){
    TFT_DC_RESET();
    TFT_Send(cmd);
}

void TFT_SendData(uint8_t data){
    TFT_DC_SET();
    TFT_Send(data);
}

void TFT_SetColumn(uint8_t column){
    TFT_SendCommand(TFT_CASET);
    TFT_SendData(0x00);
    TFT_SendData(column);
}

void TFT_SetRow(uint8_t row){
    TFT_SendCommand(TFT_RASET);
    TFT_SendData(0x00);
    TFT_SendData(row);
}

void TFT_SetRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop){
    TFT_SendCommand(TFT_RASET);
    TFT_SendData(0x00);
    TFT_SendData(Ystart);
    TFT_SendData(0x00);
    TFT_SendData(Ystop);

    TFT_SendCommand(TFT_CASET);
    TFT_SendData(0x00);
    TFT_SendData(Xstart);
    TFT_SendData(0x00);
    TFT_SendData(Xstop);
}

void TFT_Init(){

}

void TFT_DrawPixel(uint8_t Y, uint8_t X, uint16_t colour){
    TFT_SetRow(Y);
    TFT_SetColumn(X);
    TFT_SendCommand(TFT_RAMWR);
    TFT_SendData(colour >> 8);
    TFT_SendData(colour);
}

void TFT_DrawChar(uint8_t Y, uint8_t X, char c, uint16_t colourF, uint16_t colourB){
    TFT_SetRect(Y, X, Y + CHAR_HEIGHT - 1, X + CHAR_WIDTH - 1);
    TFT_SendCommand(TFT_RAMWR);
    for(uint8_t i = 0; i < CHAR_WIDTH; i++){
        for(uint8_t j = 0; j < CHAR_HEIGHT; j++){
            if(characters[(uint8_t) c][i] & (0x01 << j)){
                TFT_SendData(colourF >> 8);
                TFT_SendData(colourF);
            } else{
                TFT_SendData(colourB >> 8);
                TFT_SendData(colourB);
            }
        }
    }
}

void TFT_DrawRect(uint8_t Ystart, uint8_t Xstart, uint8_t Ystop, uint8_t Xstop, uint16_t colour){
    TFT_SetRect(Ystart, Xstart, Ystop, Xstop);
    TFT_SendCommand(TFT_RAMWR);
    for(uint16_t i = 0; i < ((Ystop - Ystart) * (Xstop - Xstart)); i++){
        TFT_SendData(colour >> 8);
        TFT_SendData(colour);
    }
}

void TFT_DumpASCII(){
    uint8_t index;
    for(uint16_t i = 0; i < 20; i++){
      for(uint16_t j = 0; j < 16; j++){
        if(i * 16 + j >= 128) return;
        index = i * 16 + j;
        TFT_DrawChar(8 * i, 8 * j, index, 0xFFFF, 0x0000);
      }
    }
}