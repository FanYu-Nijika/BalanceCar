#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "key.h"

static uint8_t key_num = 0;

uint8_t key_getnum(void){
    uint8_t temp;
    if (key_num){
        temp = key_num;
        key_num = 0;
        return temp;
    }
    return 0;
}
uint8_t key_getstate(void){
    if (HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET){
        return 1;
    }
    else if (HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET){
        return 2;
    }
    else if (HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET){
        return 3;
    }
    else (HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin) == GPIO_PIN_RESET){
        return 4;
    }
    return 0;
}
void key_tick(void){
    static uint8_t cnt, curstate, prevstate;
    ++cnt;
    if (cnt >= 20){
        cnt = 0;
        prevstate = curstate;
        curstate = key_getstate();

        if (curstate == 0 && prevstate != 0){
            key_num = prevstate;
        }
    }
}
