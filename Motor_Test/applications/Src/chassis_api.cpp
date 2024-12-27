#include "chassis_api.h"
#include "mg513_gmr500ppr.h"
#include "pid_user.h"
#include <cstdint>

CHASSIS chassis;

/**
 * @brief  设定遥控器控制底盘模式
 * @param  void
 * @retval void
 * @attention
 */
void CHASSIS::Remote_Control_Chassis_Set_Mode(void)
{
    if (1) //底盘正常模式
    {
        this->actChassis = CHASSIS_NORMAL;
    }
//    else if (1) //底盘大陀螺模式   无云台无法实现
//    {
//        actChassis = CHASSIS_GYROSCOPE;
//    }
}

/**
 * @brief  遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void CHASSIS::Remote_Control_Chassis_Mode(void)
{
    switch (actChassis)
    {
    case CHASSIS_NORMAL: //正常模式
        // this->oriChassis.Speed.vx = (fp32)rc_ctrl.rc.ch[2]; 
        // this->oriChassis.Speed.vy = (fp32)rc_ctrl.rc.ch[3]; 
        // this->oriChassis.Speed.vw = -(fp32)rc_ctrl.rc.ch[4]; 

        this->oriChassis.Speed.vx *= 1;
        this->oriChassis.Speed.vy *= 1;
        this->oriChassis.Speed.vw *= 1;
        break;

    case CHASSIS_GYROSCOPE: //小陀螺模式
        // this->oriChassis.Speed.vx = (fp32)rc_ctrl.rc.ch[2];
        // this->oriChassis.Speed.vy = (fp32)rc_ctrl.rc.ch[3];
        // this->oriChassis.Speed.vw = -330.0f;

        this->oriChassis.Speed.vx *= 1;
        this->oriChassis.Speed.vy *= 1;
        this->oriChassis.Speed.vw *= 1;
        break;
    default:
        break;
    }
}

/**
 * @brief  机器人底盘姿态获取
 * @param  void
 * @retval void
* @attention 无
 */
void CHASSIS::Chassis_Orientation_Analysis(void)
{
    //IMU欧拉角
    // this->oriChassis.Euler.yaw = - imu605.Euler.yaw;
    // this->oriChassis.Euler.yaw = - ops9.Euler.yaw;
    // this->oriChassis.Euler.pitch = imu605.Euler.pitch;
    // this->oriChassis.Euler.roll = imu605.Euler.roll;

    //里程计
    // this->oriChassis.Position.Pos_X = ops9.Position.pos_x;
    // this->oriChassis.Position.Pos_Y = ops9.Position.pos_y;

    // //获取目标Yaw
	// Get_Chassis_Yaw_Target();

	// //获取修正后的Yaw
	// Get_Chassis_Yaw_Offset();

	// //获取累计Yaw
	// Get_Chassis_Total_Yaw();

}

#define Half_X_Track_Width 1
#define Half_Y_Track_Width 2

/**
 * @brief  底盘运动解析式计算
 * @param  void
 * @retval void
* @attention  此函数是麦克纳姆轮底盘电机的速度解析式
 */
void CHASSIS::Chassis_Kinematics_Calc(void)
{
    /*	O型： */
    // mg513_gmr500ppr_motor[0].target.Speed_Motor_Target =   this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    // mg513_gmr500ppr_motor[1].target.Speed_Motor_Target = - this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    // mg513_gmr500ppr_motor[2].target.Speed_Motor_Target = - this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    // mg513_gmr500ppr_motor[3].target.Speed_Motor_Target =   this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);

    /*  X型:   */
    mg513_gmr500ppr_motor[0].target.Speed_Motor_Target =   this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    mg513_gmr500ppr_motor[1].target.Speed_Motor_Target = - this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    mg513_gmr500ppr_motor[2].target.Speed_Motor_Target = - this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);
    mg513_gmr500ppr_motor[3].target.Speed_Motor_Target =   this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Half_X_Track_Width + Half_Y_Track_Width);

}

/**
 * @brief  底盘电机输出
 * @param  void
 * @retval void
 * @attention
 */
void CHASSIS::Chassis_Loop_Out(void)
{
    for(int16_t i = 0;i < 4;i++)
    {
        mg513_gmr500ppr_motor[i].target.Pulse_Motor_Target = pid_controller.motor.Velocity_Realize(mg513_gmr500ppr_motor[i].target.Speed_Motor_Target,i);
    }
    
    for(int16_t i = 0;i < 4;i++)
    {
        mg513_gmr500ppr_motor[i].at8236_cmd.PWM_Pulse_CMD(mg513_gmr500ppr_motor[i].target.Pulse_Motor_Target);
    }
}



