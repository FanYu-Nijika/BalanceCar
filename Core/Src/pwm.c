#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "PWM.h"

void PWM_set_ccr1(uint16_t ccr){
    __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, ccr);
}
void PWM_set_ccr2(uint16_t ccr){
    __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, ccr);
}
