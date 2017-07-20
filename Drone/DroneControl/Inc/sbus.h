#ifndef __SBUS_H
#define __SBUS_H

int sbus_channel[18];

void SBus_Init(void);
void SBus_Read(volatile uint16_t *rc);

#endif
