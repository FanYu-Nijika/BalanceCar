#include "main.h"
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "mpu6050.h"
#include "iic.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0

void mpu6050_writereg(uint8_t reg_addr, uint8_t data){ //向MPU6050寄存器写入数据
    iic_start();
    iic_sendbyte(MPU6050_ADDRESS);
    iic_receiveack();
    iic_sendbyte(reg_addr);
    iic_receiveack();
    iic_sendbyte(data);
    iic_receiveack();
    iic_stop();
}
void mpu6050_readreg(uint8_t reg_addr, uint8_t* dataarr, uint8_t len){ //从MPU6050寄存器读取数据
    iic_start();
    iic_sendbyte(MPU6050_ADDRESS);
    iic_receiveack();
    iic_sendbyte(reg_addr);
    iic_receiveack();
    iic_start();
    iic_sendbyte(MPU6050_ADDRESS | 0x01);
    iic_receiveack();
    for(int i = 0; i < len; i++){
        dataarr[i] = iic_receivebyte(i != len - 1 ? 0 : 1);
    }
    iic_stop();
}
void mpu6050_init(void){
    mpu6050_writereg(MPU6050_PWR_MGMT_1, 0x01); //唤醒MPU6050
    mpu6050_writereg(MPU6050_PWR_MGMT_2, 0x00); //关闭传感器
    mpu6050_writereg(MPU6050_SMPLRT_DIV, 0x07); //设置采样率为1kHz / (1 + 7) = 125Hz
    mpu6050_writereg(MPU6050_CONFIG, 0x00); //设置数字低通滤波器为0（不使用）
    mpu6050_writereg(MPU6050_GYRO_CONFIG, 0x18); //设置陀螺仪量程为2000°/s
    mpu6050_writereg(MPU6050_ACCEL_CONFIG, 0x18); //设置加速度计量程为16g
}
void mpu6050_getdata()(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ){
    uint8_t DataArray[14];
    mpu6050_readreg(MPU6050_ACCEL_XOUT_H, DataArray, 14);
    *AccX = (DataArray[0] << 8) | DataArray[1];
	*AccY = (DataArray[2] << 8) | DataArray[3];
	*AccZ = (DataArray[4] << 8) | DataArray[5];

	*GyroX = (DataArray[8] << 8) | DataArray[9];
	*GyroY = (DataArray[10] << 8) | DataArray[11];
	*GyroZ = (DataArray[12] << 8) | DataArray[13];
}