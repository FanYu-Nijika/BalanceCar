#ifndef __IIC_H
#define __IIC_H
void iic_start(void);
void iic_stop(void);
void iic_sendbyte(uint8_t byte);
uint8_t iic_receivebyte(void);
void iic_sendack(uint8_t ackbit);
uint8_t iic_receiveack(void);

#endif
