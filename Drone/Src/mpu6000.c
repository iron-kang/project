#include "mpu6000.h"

#define MPU6000_SPI_ENABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define MPU6000_SPI_DISABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

HAL_StatusTypeDef status;
SPI_HandleTypeDef *hspi;
uint8_t addr, dat;
uint8_t buf[14];

void MPU6000_Init(SPI_HandleTypeDef *spi)
{
	hspi = spi;

	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_1;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x80;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(150);

	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_1;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x03;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_USER_CTRL;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x10;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_2;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_SMPLRT_DIV;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_CONFIG;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_ACCEL_CONFIG;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x0;//0x08;		//1000
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_GYRO_CONFIG;
	HAL_SPI_Transmit(hspi, &addr, 1, 50);
	dat = 0x0;//0x10;		//4G
	HAL_SPI_Transmit(hspi, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(100);
 
}

void MPU6000_Raw(MPU6000_RAW *raw)
{
	MPU6000_SPI_ENABLE;
	addr = MPU6000_ACCEL_XOUT_H | 0x80;
	status = HAL_SPI_Transmit(hspi, &addr, 1, 50);
	status = HAL_SPI_Receive(hspi, buf, 14, 50);
	MPU6000_SPI_DISABLE;
	
	raw->ax = (((int16_t)buf[0]) << 8) | buf[1];
	raw->ay = (((int16_t)buf[2]) << 8) | buf[3];
	raw->az = (((int16_t)buf[4]) << 8) | buf[5];
	raw->temp = (((int16_t)buf[6]) << 8) | buf[7];
	raw->gx = (((int16_t)buf[8]) << 8) | buf[9];
	raw->gy = (((int16_t)buf[10]) << 8) | buf[11];
	raw->gz = (((int16_t)buf[12]) << 8) | buf[13];
}

