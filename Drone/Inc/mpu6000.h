#ifndef __MPU6000_H
#define __MPU6000_H

#include "stm32f1xx_hal.h"

#define MPU6000_CONFIG          0x1A
#define MPU6000_SMPLRT_DIV      0x19
#define MPU6000_GYRO_CONFIG     0x1B
#define MPU6000_ACCEL_CONFIG    0x1C
#define MPU6000_FIFO_EN         0x23
#define MPU6000_INT_PIN_CFG     0x37
#define MPU6000_INT_ENABLE	    0x38
#define MPU6000_INT_STATUS	    0x3A
#define MPU6000_ACCEL_XOUT_H    0x3B
#define MPU6000_ACCEL_XOUT_L    0x3C
#define MPU6000_ACCEL_YOUT_H    0x3D
#define MPU6000_ACCEL_YOUT_L    0x3E
#define MPU6000_ACCEL_ZOUT_H    0x3F
#define MPU6000_ACCEL_ZOUT_L    0x40
#define MPU6000_TEMP_OUT_H	    0x41
#define MPU6000_TEMP_OUT_L	    0x42
#define MPU6000_GYRO_XOUT_H	    0x43
#define MPU6000_GYRO_XOUT_L	    0x44
#define MPU6000_GYRO_YOUT_H	    0x45
#define MPU6000_GYRO_YOUT_L	    0x46
#define MPU6000_GYRO_ZOUT_H	    0x47
#define MPU6000_GYRO_ZOUT_L	    0x48
#define MPU6000_USER_CTRL       0x6A
#define MPU6000_PWR_MGMT_1	    0x6B
#define MPU6000_PWR_MGMT_2	    0x6C
#define MPU6000_FIFO_COUNTH	    0x72
#define MPU6000_FIFO_COUNTL	    0x73
#define MPU6000_FIFO_R_W        0x74
#define MPU6000_WHOAMI	        0x75

typedef struct {
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t gx;
	int16_t gy;
	int16_t gz;
	int16_t temp;
} MPU6000_INFO;

typedef struct {
    float yaw;
    float pitch;
    float roll;
} MPU6000_Attitude;

typedef struct {
    float x;
    float y;
    float z;
} MPU6000_Gyro;

void MPU6000_Init(void);
void MPU6000_Raw(MPU6000_INFO *raw);
void MPU6000_Filter(MPU6000_INFO *raw, MPU6000_INFO *filter);
void MPU6000_AttitudeFusion(MPU6000_Gyro *gyro, MPU6000_INFO *filter, MPU6000_Attitude *attitude);

#endif

