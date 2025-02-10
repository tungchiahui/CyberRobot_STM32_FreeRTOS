#include "ros2_comm.h"
#include "udb.h"
#include "serial_pack.h"
//#include "mpu6050.h"
#include "mg513_gmr500ppr.h"
#include "odom_mg513.h"

Chassis_Cmd_Vel_t cmd_vel_;


uint8_t ros2_rx_buffer[39];
extern uint8_t udb_rx_buffer[12];

//extern osSemaphoreId IMU_ROS2_SemapHandle;
extern osSemaphoreId_t MOTOR_ROS2_SemapHandle;

extern "C"
void ros2_uart_task(void const * argument)
{
	uint32_t PreviousWakeTime = osKernelGetTickCount();
	for(;;)
	{
		
//		xSemaphoreTake(IMU_ROS2_SemapHandle,portMAX_DELAY);
		
//		xSemaphoreTake(MOTOR_ROS2_SemapHandle,portMAX_DELAY);
		osSemaphoreAcquire(MOTOR_ROS2_SemapHandle, osWaitForever);
		
		// bool bool_buffer[] = {1, 0, 1, 0};
		// int8_t int8_buffer[] = {0x11,0x22};
		// int16_t int16_buffer[] = {2000,6666};
		// int32_t int32_buffer[] = {305419896};
		fp32 fp32_buffer[] = {
							// mpu6050.data.Accel.X,mpu6050.data.Accel.Y,mpu6050.data.Accel.Z,
							// mpu6050.data.Gyro.X,mpu6050.data.Gyro.Y,mpu6050.data.Gyro.Z,
							// mpu6050.data.Euler.roll,mpu6050.data.Euler.pitch,mpu6050.data.Euler.yaw,
							// mpu6050.data.Quat.q[0],mpu6050.data.Quat.q[1],mpu6050.data.Quat.q[2],mpu6050.data.Quat.q[3],
							// mpu6050.data.Temp,
//							mg513_gmr500ppr_motor[0].encoder.motor_data.motor_speed,mg513_gmr500ppr_motor[1].encoder.motor_data.motor_speed,
//							mg513_gmr500ppr_motor[2].encoder.motor_data.motor_speed,mg513_gmr500ppr_motor[3].encoder.motor_data.motor_speed,
//							mg513_gmr500ppr_motor[0].encoder.motor_data.motor_position,mg513_gmr500ppr_motor[1].encoder.motor_data.motor_position,
//							mg513_gmr500ppr_motor[2].encoder.motor_data.motor_position,mg513_gmr500ppr_motor[3].encoder.motor_data.motor_position,
							odom_motor_.vx,odom_motor_.vy,odom_motor_.vw,odom_motor_.yaw,odom_motor_.dt,odom_motor_.x_position,odom_motor_.y_position
							};

		// 发送数据
        serial_pack_.tx.Data_Pack(0x01, 
                                 nullptr, 0,
                                 nullptr, 0,
                                 nullptr, 0,
                                 nullptr, 0,
                                 fp32_buffer, sizeof(fp32_buffer) / sizeof(fp32));
		//每2ms发一次
		PreviousWakeTime += 2;
		osDelayUntil(PreviousWakeTime);
	}
}


// extern "C"
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
// 	//蓝牙遥控器
// 	if(huart->Instance == USART2)
// 	{
// 		udb.rx.Data_Analysis(udb_rx_buffer);
// 	}
	
// 	//ROS2通信
// 	if(huart->Instance == USART3)
// 	{
// 		serial_pack_.rx.Data_Analysis(ros2_rx_buffer,0x01,0,0,0,0,8);
// 	}
// }

//uint8_t rx_buffer[12];

extern "C"
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	//蓝牙遥控器
	if(huart->Instance == USART2)
	{
		if(Size == 12)
		{
			for(int16_t i = 0;i < 12;i++)
			{
//				rx_buffer[i] = udb_rx_buffer[i];
				udb.rx.Data_Analysis(&udb_rx_buffer[i]);
			}
		}
	}
	
	//ROS2通信
	if(huart->Instance == USART3)
	{
		if(Size == 39)
		{
			for(int16_t i = 0;i < 39;i++)
			{
				serial_pack_.rx.Data_Analysis(&ros2_rx_buffer[i],0x01,0,0,0,0,8);
			}
		}
	}
}