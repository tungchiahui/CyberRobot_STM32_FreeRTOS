#include "ros2_comm.h"
#include "udb.h"
#include "serial_pack.h"
#include "mpu6050.h"
#include "mg513_gmr500ppr.h"

uint8_t ros2_rx_buffer[1];
extern uint8_t udb_rx_buffer[1];


extern "C"
void ros2_uart_task(void const * argument)
{
	// bool bool_buffer[] = {1, 0, 1, 0};
    // int8_t int8_buffer[] = {0x11,0x22};
    // int16_t int16_buffer[] = {2000,6666};
    // int32_t int32_buffer[] = {305419896};
    fp32 fp32_buffer[] = {mpu6050.data.Accel.X,mpu6050.data.Accel.Y,mpu6050.data.Accel.Z,
						  mpu6050.data.Gyro.X,mpu6050.data.Gyro.Y,mpu6050.data.Gyro.Z,
						  mpu6050.data.Euler.yaw,mpu6050.data.Euler.pitch,mpu6050.data.Euler.roll,
						  mpu6050.data.Quat.q[0],mpu6050.data.Quat.q[1],mpu6050.data.Quat.q[2],mpu6050.data.Quat.q[3],
						 mpu6050.data.Temp,
						 mg513_gmr500ppr_motor[0].encoder.motor_data.motor_speed,mg513_gmr500ppr_motor[1].encoder.motor_data.motor_speed,
						 mg513_gmr500ppr_motor[2].encoder.motor_data.motor_speed,mg513_gmr500ppr_motor[3].encoder.motor_data.motor_speed,
						 mg513_gmr500ppr_motor[0].encoder.motor_data.motor_position,mg513_gmr500ppr_motor[1].encoder.motor_data.motor_position,
						 mg513_gmr500ppr_motor[2].encoder.motor_data.motor_position,mg513_gmr500ppr_motor[3].encoder.motor_data.motor_position};

	uint32_t PreviousWakeTime = osKernelSysTick();
	for(;;)
	{
		// 发送数据
        serial_pack_.tx.Data_Pack(0x01, 
                                 nullptr, 0,
                                 nullptr, 0,
                                 nullptr, 0,
                                 nullptr, 0,
                                 fp32_buffer, sizeof(fp32_buffer) / sizeof(fp32));  
		osDelayUntil(&PreviousWakeTime,1);
	}
}


extern "C"
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//蓝牙遥控器
	if(huart->Instance == USART2)
	{
		udb.rx.Data_Analysis(udb_rx_buffer);
	}
	
	//ROS2通信
	if(huart->Instance == USART3)
	{
		//serial_pack_.rx.Data_Analysis(ros2_rx_buffer,0x01,16,2,2,1,1);
	}
}