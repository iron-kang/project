#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"

void UART3_Init(void);
void UART3_Print(char *str);
void UART3_Read(char *buf, uint8_t size);

#endif
