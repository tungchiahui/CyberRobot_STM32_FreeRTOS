#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
	
//in the beginning of task ,wait a time
//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 357

//chassis task control time  1ms
//底盘任务控制间隔 1ms
#define CHASSIS_CONTROL_TIME_MS 1

	
#ifdef __cplusplus	
}
#endif

#endif
