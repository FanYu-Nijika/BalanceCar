#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "encoder.h"

void encoder_init(void){
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}
int16_t encoder_get(uint8_t encoder){
    int16_t temp = 0;
    if (encoder == 1){
        temp = __HAL_TIM_GET_COUNTER(&htim3);
        __HAL_TIM_SET_COUNTER(&htim3, 0);
    }
    else {
        temp = __HAL_TIM_GET_COUNTER(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
    }
    return temp;
}
