#include "common.h"
//https://www.amobbs.com/forum.php?mod=viewthread&action=printable&tid=5428622
#define SBUS_SYNCBYTE 0x0F

UART_HandleTypeDef huart1;
static uint8_t sbus[25] = {0};
static uint8_t sbusIndex = 0;
uint8_t tmp;

void SBus_Init(void)
{
	huart1.Instance = USART1;
    huart1.Init.BaudRate = 100000;//115200; 
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
}

void SBus_Read(volatile int *channel)
{
	sbusIndex = 0;
	
	while (sbusIndex != 25)
	{
		HAL_UART_Receive(&huart1, &tmp, 1, 1000);
		if(sbusIndex == 0 && tmp != SBUS_SYNCBYTE)
			continue;
		sbus[sbusIndex++] = tmp;
	}
	
	channel[0]  = (int) lround(((sbus[1]    |sbus[2]<<8)               & 0x07FF) / 9.92) - 100;
	channel[1]  = (int) lround(((sbus[2]>>3 |sbus[3]<<5)               & 0x07FF) / 9.92) - 100;
	channel[2]  = (int) lround(((sbus[3]>>6 |sbus[4]<<2|sbus[5]<<10)   & 0x07FF) / 9.92) - 100;
	channel[3]  = (int) lround(((sbus[5]>>1 |sbus[6]<<7)               & 0x07FF) / 9.92) - 100;
	channel[4]  = (int) lround(((sbus[6]>>4 |sbus[7]<<4)               & 0x07FF) / 9.92) - 100;
	channel[5]  = (int) lround(((sbus[7]>>7 |sbus[8]<<1|sbus[9]<<9)    & 0x07FF) / 9.92) - 100;
	channel[6]  = (int) lround(((sbus[9]>>2 |sbus[10]<<6)              & 0x07FF) / 9.92) - 100;
	channel[7]  = (int) lround(((sbus[10]>>5|sbus[11]<<3)              & 0x07FF) / 9.92) - 100;
	channel[8]  = (int) lround(((sbus[12]   |sbus[13]<<8)              & 0x07FF) / 9.92) - 100;
	channel[9]  = (int) lround(((sbus[13]>>3|sbus[14]<<5)              & 0x07FF) / 9.92) - 100;
	channel[10] = (int) lround(((sbus[14]>>6|sbus[15]<<2|sbus[16]<<10) & 0x07FF) / 9.92) - 100;
	channel[11] = (int) lround(((sbus[16]>>1|sbus[17]<<7)              & 0x07FF) / 9.92) - 100;
	channel[12] = (int) lround(((sbus[17]>>4|sbus[18]<<4)              & 0x07FF) / 9.92) - 100;
	channel[13] = (int) lround(((sbus[18]>>7|sbus[19]<<1|sbus[20]<<9)  & 0x07FF) / 9.92) - 100;
	channel[14] = (int) lround(((sbus[20]>>2|sbus[21]<<6)              & 0x07FF) / 9.92) - 100;
	channel[15] = (int) lround(((sbus[21]>>5|sbus[22]<<3)              & 0x07FF) / 9.92) - 100;

	if ((sbus[23]) & 0x0001)       channel[16] = (int) lround(2047 / 9.92) - 100; else channel[16] = 0;
	if ((sbus[23] >> 1) & 0x0001)  channel[17] = (int) lround(2047 / 9.92) - 100; else channel[17] = 0;
}
	
