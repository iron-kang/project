//https://github.com/jihlein/AQ32Plus
#include "common.h"

#define MPU6000_SPI_ENABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define MPU6000_SPI_DISABLE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define Pi	3.1415927f 
#define RawData_to_Angle	0.0610351f
#define RawData_to_Radian	0.0010653f
#define Kp 	1.0f
#define Ki 	0.001f
#define halfT 0.0005f
#define T	0.001f

HAL_StatusTypeDef status;
SPI_HandleTypeDef hspi1;
uint8_t addr, dat;
uint8_t buf[14];
float IIR_FACTOR = 0.001 /( 0.001 + 1/(2.0f*Pi*10.0) );
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float exInt = 0, eyInt = 0, ezInt = 0; 
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

void Get_Radian(MPU6000_INFO *filter, MPU6000_Gyro *gyro)
{
    gyro->x = (float)(filter->gx * RawData_to_Radian);
	gyro->y = (float)(filter->gy * RawData_to_Radian);
	gyro->z = (float)(filter->gz * RawData_to_Radian);
}

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
	dat = 0x18;//0x08;		//1000
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(1);
	
	MPU6000_SPI_ENABLE;
	addr = MPU6000_GYRO_CONFIG;
	HAL_SPI_Transmit(&hspi1, &addr, 1, 50);
	dat = 0x1F;//0x10;		//4G
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	MPU6000_SPI_DISABLE;
	HAL_Delay(100);
 
}

void MPU6000_Raw(MPU6000_INFO *raw)
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

void MPU6000_Filter(MPU6000_INFO *raw, MPU6000_INFO *filter)
{
    static int16_t Filter_x[2],Filter_y[2],Filter_z[2];
	static uint8_t Filter_count;
	int32_t Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;
	uint8_t i=0;
	
	Filter_x[Filter_count] = raw->gx;
	Filter_y[Filter_count] = raw->gy;
	Filter_z[Filter_count] = raw->gz;

	for(i=0;i<2;i++)
	{
		Filter_sum_x += Filter_x[i];
		Filter_sum_y += Filter_y[i];
		Filter_sum_z += Filter_z[i];
	}	
	
	filter->ax = Filter_sum_x / 2;
	filter->ay = Filter_sum_y / 2;
	filter->az = Filter_sum_z / 2;
	
	Filter_count++;
	if(Filter_count == 2)
		Filter_count=0;
    
    filter->ax = filter->ax + IIR_FACTOR*(raw->ax - filter->ax); 
	filter->ay = filter->ay + IIR_FACTOR*(raw->ay - filter->ay); 
	filter->az = filter->az + IIR_FACTOR*(raw->az - filter->az);
}

void MPU6000_AttitudeFusion(MPU6000_Gyro *gyro, MPU6000_INFO *filter, MPU6000_Attitude *attitude)
{   
    float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;	
    ax = filter->ax;
    ay = filter->ay;
    az = filter->az;
    gx = gyro->x;
    gy = gyro->y;
    gz = gyro->z;
 
    Get_Radian(filter, gyro);
	norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	ex = ay*vz - az*vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;

	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;

	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 
			
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	
	attitude->yaw  +=  filter->gz * RawData_to_Angle * 0.001f;
}

