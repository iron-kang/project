#include "common.h"

UART_HandleTypeDef huart3;

void UART3_Init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
}

void UART3_Print(char *str)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), 100);
}
