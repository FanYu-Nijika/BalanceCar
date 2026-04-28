#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "iic.h"

void iic_start(void){
    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_RESET);
}
void iic_stop(void){
    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);
}
void iic_sendbyte(uint8_t byte){
    for (int i = 0; i < 8; ++i){
        HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, (!!(Byte & (0x80 >> i))));
        HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_RESET);
    }
}
uint8_t iic_receivebyte(void){
    uint8_t ans = 0x00;
    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_SET);
    for (int i = 0; i < 8; ++i){
        HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(MSDA_GPIO_Port, MSDA_Pin)){ ans |= (0x80 >> i);}
		HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_RESET);
	}
	return ans;
}
void iic_sendack(uint8_t ackbit){
    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, ackbit);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_RESET);
}
uint8_t iic_receiveack(void){
	uint8_t AckBit;
	// MyI2C_W_SDA(1);
    HAL_GPIO_WritePin(MSDA_GPIO_Port, MSDA_Pin, GPIO_PIN_SET);
	// MyI2C_W_SCL(1);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_SET);
	AckBit = HAL_GPIO_ReadPin(MSDA_GPIO_Port, MSDA_Pin);
	// MyI2C_W_SCL(0);
    HAL_GPIO_WritePin(MSCL_GPIO_Port, MSCL_Pin, GPIO_PIN_RESET);
	return AckBit;
}
