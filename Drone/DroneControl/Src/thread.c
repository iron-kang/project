#include "common.h"

osThreadId thread_1000Hz;
MPU6000_INFO raw, filter;
volatile uint16_t channel[18];
char buffer[100];

void Thread_1000Hz(void const *argument);

void Thread_Start(void)
{
    osThreadDef(THREAD_1000HZ, Thread_1000Hz, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	thread_1000Hz = osThreadCreate(osThread(THREAD_1000HZ), NULL);
    
    osKernelStart();
}

void Thread_1000Hz(void const *argument)
{
	uint32_t PreviousWakeTime = osKernelSysTick();
	uint8_t i;

	for (;;)
    {
        MPU6000_Raw(&raw);
        MPU6000_Filter(&raw, &filter);
        SBus_Read(channel);
        for (i = 0; i < 18; i++)
        {
        	sprintf(buffer, "ch%d: %d\n", i+1, (int) lround(channel[i] / 9.92) - 100);
        	UART3_Print(buffer);
        }
        //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        osDelayUntil(&PreviousWakeTime, 1);
    }
}
