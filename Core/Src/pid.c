#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "pid.h"

void pid_reset(Pid_t *pid){
    pid->setpoint = 0;
    pid->feedback = 0;
    pid->integral = 0;
    pid->output = 0;

    pid->error = 0;
    pid->last_error = 0;
}
void pid_init(Pid_t *pid, float kp, float ki, float kd, float max_out, float max_int){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->max_output = max_out;
    pid->max_integral = max_int;

    pid_reset();
}
float pid_calc(Pid_t *pid, float set, float feedback){
    pid->setpoint = set;
    pid->feedback = feedback;

    pid->error = set - feedback;
    pid->integral += pid->error;
    if (pid->integral >= pid->max_integral) pid->integral = pid->max_integral;
    if (pid->integral <= -pid->max_integral) pid->integral = -pid->max_integral;

    float dev = pid->error - pid->last_error;
    pid->output = pid->kp*pid->error + pid->ki*pid->integral + pid->kd*dev;
    if (pid->output >= pid->max_output) pid->output = pid->max_output;
    if (pid->output <= -pid->max_output) pid->output = -pid->max_output;

    pid->last_error = pid->error;
    return pid->output;
}

