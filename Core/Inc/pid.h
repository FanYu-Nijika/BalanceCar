#ifndef __PID_H
#define __PID_H

typedef struct {
    float kp;
    float ki;
    float kd;

    float setpoint;
    float feedback;
    float integral;
    float output;

    float error;
    float last_error;

    float max_output;
    float max_integral;
} Pid_t;

void pid_reset(Pid_t *pid);
void pid_init(Pid_t *pid, float kp, float ki, float kd, float max_out, float max_int);
float pid_calc(Pid_t *pid, float set, float feedback);

#endif
