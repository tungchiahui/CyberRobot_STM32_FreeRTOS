#ifndef __CHASSIS_API_H_
#define __CHASSIS_API_H_

#include "chassis_task.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
	
//底盘模式
typedef enum
{
    CHASSIS_NORMAL   = 0,      //底盘遥控行走
    CHASSIS_GYROSCOPE = 1,	   //小陀螺模式
		CHASSIS_ROS2_CMD = 2,			 //ROS2接管模式
}eChassisAction;


typedef struct
{
	__packed struct
	{
		fp32 vx;
		fp32 vy;
		fp32 vw;
	}Speed;
	
	__packed struct
	{
		fp32 yaw;
		fp32 pitch;
		fp32 roll;
		
		fp32 yaw_offset;
		fp32 yaw_last_angle;
		fp32 yaw_rel;   //yaw_relative
		fp32 yaw_round_cnt;
		fp32 yaw_total;
		
		fp32 yaw_target;
	}Euler;
	
	__packed struct
	{
		fp32 Pos_X;
		fp32 Pos_Y;
		
		fp32 Pos_X_offset;
		fp32 Pos_Y_offset;
		
		fp32 Pos_X_Target;
		fp32 Pos_Y_Target;
	}Position;

}Chassis_ori_measure_t;


class CHASSIS
{
    public:
    Chassis_ori_measure_t oriChassis;
    eChassisAction actChassis;

    void Remote_Control_Chassis_Set_Mode(void);
    void Remote_Control_Chassis_Mode(void);

    void Chassis_Orientation_Analysis(void);

    void Chassis_Kinematics_Calc(void);

    void Chassis_Loop_Out(void);


};

extern CHASSIS chassis;

#ifdef __cplusplus	
}
#endif

#endif
