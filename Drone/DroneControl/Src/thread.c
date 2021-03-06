#include "common.h"

osThreadId thread_1000Hz;
osThreadId thread_10Hz;
MPU6000_INFO raw, filter;
GPS_M8N gps;
volatile int channel[18];
char buffer[100];

void Thread_1000Hz(void const *argument);
void Thread_10Hz(void const *argument);

void Thread_Start(void)
{
    osThreadDef(THREAD_1000HZ, Thread_1000Hz, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	thread_1000Hz = osThreadCreate(osThread(THREAD_1000HZ), NULL);

	osThreadDef(THREAD_10Hz, Thread_10Hz, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	thread_10Hz = osThreadCreate(osThread(THREAD_10Hz), NULL);
    
    osKernelStart();
}

void Thread_10Hz(void const *argument)
{
	uint32_t PreviousWakeTime = osKernelSysTick();
	uint8_t i;
	char c;

	while (1)
	{
		SBus_Read(channel);
		for (i = 0; i < 18; i++)
		{
			memcpy(buffer, channel, 18*sizeof(int));
//			sprintf(buffer, "ch%d: %d\n", i+1, sbus_channel[i]);
//			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer));
//			UART3_Print(buffer);
		}
		UART3_Read(&c, 1);
		if (GPS_Encode(c)) {
			gps.altitude;
			gps.latitude;
			gps.longitude;
			gps.sats_in_use;
		}
		CDC_Transmit_FS((uint8_t *)buffer, 18*sizeof(int));
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		osDelayUntil(&PreviousWakeTime, 10);
	}
}

void Thread_1000Hz(void const *argument)
{
	uint32_t PreviousWakeTime = osKernelSysTick();

	for (;;)
    {
        MPU6000_Raw(&raw);
        MPU6000_Filter(&raw, &filter);


        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        osDelayUntil(&PreviousWakeTime, 1);
    }
}
