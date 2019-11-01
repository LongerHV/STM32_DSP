#include "tft.h"

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
    TFT_SendCommand(TFT_COLUMN_ADDRESS_SET);
    TFT_SendData(0x00);
    TFT_SendData(column);
}

void TFT_SetRow(uint8_t row){
    TFT_SendCommand(TFT_ROW_ADDRESS_SET);
    TFT_SendData(0x00);
    TFT_SendData(row);
}

void TFT_Init(){

}

void TFT_DrawPixel(uint8_t Y, uint8_t X, uint16_t colour){
    TFT_SetRow(Y);
    TFT_SetColumn(X);
    TFT_SendCommand(TFT_MEMORY_WRITE);
    TFT_SendData(colour >> 8);
    TFT_SendData(colour);
}

void TFT_DrawChar(uint8_t Y, uint8_t X, char c){
    uint8_t col;
    for(uint8_t i = 0; i < CHAR_WIDTH; i++){
        TFT_SetRow(Y + i);
        for(uint8_t j = 0; j < CHAR_HEIGHT; j++){
            TFT_SetColumn(X + j);
            TFT_SendCommand(TFT_MEMORY_WRITE);
            if(characters[(uint8_t) c][i] & (0x01 << j)){
                TFT_SendData(0xFF);
                TFT_SendData(0xFF);
            } else{
                TFT_SendData(0x00);
                TFT_SendData(0x00);
            }
        }
    }
}
