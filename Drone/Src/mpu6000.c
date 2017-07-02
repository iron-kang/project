#include "mpu6000.h"

#define MPU6000_SPI_ENABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define MPU6000_SPI_DISABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

HAL_StatusTypeDef status;
SPI_HandleTypeDef hspi1;
uint8_t addr, dat;
uint8_t buf[14];

void MPU6000_SPI_Init()
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }

}

void MPU6000_Init(void)
{
    MPU6000_SPI_Init();
        
	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_1;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x80;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(150);

	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_1;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x03;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_USER_CTRL;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x10;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_PWR_MGMT_2;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_SMPLRT_DIV;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_CONFIG;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x0;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_ACCEL_CONFIG;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x0;//0x08;		//1000
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_GYRO_CONFIG;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x0;//0x10;		//4G
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(100);
 
}

void MPU6000_Raw(MPU6000_RAW *raw)
{
	MPU6000_SPI_ENABLE;
	addr = MPU6000_ACCEL_XOUT_H | 0x80;
	status = HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	status = HAL_SPI_Receive(&hspi1, buf, 14, 50);
	MPU6000_SPI_DISABLE;
	
	raw->ax = (((int16_t)buf[0]) << 8) | buf[1];
	raw->ay = (((int16_t)buf[2]) << 8) | buf[3];
	raw->az = (((int16_t)buf[4]) << 8) | buf[5];
	raw->temp = (((int16_t)buf[6]) << 8) | buf[7];
	raw->gx = (((int16_t)buf[8]) << 8) | buf[9];
	raw->gy = (((int16_t)buf[10]) << 8) | buf[11];
	raw->gz = (((int16_t)buf[12]) << 8) | buf[13];
}

