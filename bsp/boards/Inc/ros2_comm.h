#ifndef __ROS2_COMM_TASK_H_
#define __ROS2_COMM_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"

typedef struct
{
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Linear;
	
	__packed struct
	{
		fp32 X;
		fp32 Y;
		fp32 Z;
	}Angular;

}Chassis_Cmd_Vel_t;
	
extern Chassis_Cmd_Vel_t cmd_vel_;
	
#ifdef __cplusplus
}
#endif
	
#endif
