#ifndef __SBUS_H
#define __SBUS_H

#include "stm32f1xx_hal.h"

void SBus_Init(void);
void SBus_Read(uint16_t *rc);

#endif
