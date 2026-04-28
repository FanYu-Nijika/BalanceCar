#ifndef __MPU6050_H
#define __MPU6050_H
void mpu6050_writereg(uint8_t reg_addr, uint8_t data);
void mpu6050_readreg(uint8_t reg_addr, uint8_t* dataarr, uint8_t len);
void mpu6050_init(void);
void mpu6050_getdata()(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
