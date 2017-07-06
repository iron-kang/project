#include "thread.h"
#include "cmsis_os.h"
#include "mpu6000.h"
#include "task.h"
#include "sbus.h"

osThreadId LEDThread1Handle;
osThreadId thread_1000Hz;
MPU6000_INFO raw, filter;
volatile uint16_t rcValue[18]; 

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
	for (;;)
    {
        MPU6000_Raw(&raw);
        MPU6000_Filter(&raw, &filter);
        //SBus_Read(rcValue);
        //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        osDelayUntil(&PreviousWakeTime, 1);
    }
}
