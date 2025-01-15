#include "ros2_comm.h"
#include "udb.h"


uint8_t ros2_rx_buffer[1];
extern uint8_t udb_rx_buffer[1];


extern "C"
void ros2_uart_task(void const * argument)
{
	uint32_t PreviousWakeTime = osKernelSysTick();
	for(;;)
	{
		osDelayUntil(&PreviousWakeTime,200);
	}
}


extern "C"
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		udb.rx.Data_Analysis(udb_rx_buffer);
	}
	if(huart->Instance == USART3)
	{
		udb.rx.Data_Analysis(ros2_rx_buffer);
	}
}