#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "led.h"

void led_on(void){
    HAL_GPIO_WritePin(L_GPIO_Port, L_Pin, GPIO_PIN_RESET)
}
void led_off(void){
    HAL_GPIO_WritePin(L_GPIO_Port, L_Pin, GPIO_PIN_SET)
}
void led_turn(void){
    if (HAL_GPIO_ReadPin(L_GPIO_Port, L_Pin) == GPIO_PIN_SET){
        led_on();
    }
    else {
        led_off();
    }
}