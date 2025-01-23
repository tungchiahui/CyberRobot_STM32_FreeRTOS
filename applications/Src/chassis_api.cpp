#include "chassis_api.h"
#include "mg513_gmr500ppr.h"
#include "pid_user.h"
#include <cstdint>
#include "udb.h"
#include "ros2_comm.h"

CHASSIS chassis;

/**
 * @brief  设定遥控器控制底盘模式
 * @param  void
 * @retval void
 * @attention
 */
void CHASSIS::Remote_Control_Chassis_Set_Mode(void)
{
    if(udb.rx.apply.rc.s[0] == false && udb.rx.apply.rc.s[1] == false) //底盘正常模式
    {
        this->actChassis = CHASSIS_NORMAL;
    }
    else if(udb.rx.apply.rc.s[0] == false && udb.rx.apply.rc.s[1] == true) //底盘大陀螺模式
   {
       actChassis = CHASSIS_GYROSCOPE;
   }
		else if(udb.rx.apply.rc.s[0] == false && udb.rx.apply.rc.s[1] == false) //ROS2接管模式
   {
       actChassis = CHASSIS_ROS2_CMD;
   }
}

/**
 * @brief  遥控器控制方式
 * @param  void
 * @retval void
 * @attention
 */
void CHASSIS::Remote_Control_Chassis_Mode(void)
{
	//尽量统一单位
	//vy单位为m/s
	//vx单位为m/s
	//vw单位为rad/s
	//本底盘最大速度约为1.26m/s
	//最大旋转速度约为7.01rad/s
	
    switch (actChassis)
    {
    case CHASSIS_NORMAL: //正常模式
        this->oriChassis.Speed.vx =   (fp32)udb.rx.apply.rc.ch[0]; 
        this->oriChassis.Speed.vy =   (fp32)udb.rx.apply.rc.ch[1]; 
        this->oriChassis.Speed.vw = - (fp32)udb.rx.apply.rc.ch[2]; 
		
        this->oriChassis.Speed.vx *= (1.26f / 660.0f);
        this->oriChassis.Speed.vy *= (1.26f / 660.0f);
        this->oriChassis.Speed.vw *=-(7.01f / 660.0f);
        break;

    case CHASSIS_GYROSCOPE: //小陀螺模式
        this->oriChassis.Speed.vx =   (fp32)udb.rx.apply.rc.ch[0]; 
        this->oriChassis.Speed.vy =   (fp32)udb.rx.apply.rc.ch[1]; 
        this->oriChassis.Speed.vw = - (fp32)udb.rx.apply.rc.ch[3]; 

        this->oriChassis.Speed.vx *= (1.26f / 660.0f);
        this->oriChassis.Speed.vy *= (1.26f / 660.0f);
        this->oriChassis.Speed.vw *=-(7.01f / 660.0f);
		
				this->oriChassis.Speed.vx *= 0;
        this->oriChassis.Speed.vy *= 0;
        this->oriChassis.Speed.vw *= 0;
        break;
		case CHASSIS_ROS2_CMD: //ROS2接管模式
        this->oriChassis.Speed.vx =   cmd_vel_.Linear.X;
        this->oriChassis.Speed.vy =   cmd_vel_.Linear.Y;
        this->oriChassis.Speed.vw =   cmd_vel_.Angular.Z;

//        this->oriChassis.Speed.vx *= (1.26f / 660.0f);
//        this->oriChassis.Speed.vy *= (1.26f / 660.0f);
//        this->oriChassis.Speed.vw *=-(7.01f / 660.0f);
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

//（单位：米）
//轮距
const fp32 Wheel_Spacing = 0.093f;
//轴距
const fp32 Alex_Spacing = 0.085f;
//轮子半径
const fp32 Wheel_Radius  = 0.0375f;

/**
 * @brief  底盘运动解析式计算
 * @param  void
 * @retval void
* @attention  此函数是麦克纳姆轮底盘电机的速度解析式
 */
void CHASSIS::Chassis_Kinematics_Calc(void)
{
		//速度的单位是rpm
	
    /*	O型： */
    // mg513_gmr500ppr_motor[0].target.Speed_Motor_Target =   this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    // mg513_gmr500ppr_motor[1].target.Speed_Motor_Target =   this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    // mg513_gmr500ppr_motor[2].target.Speed_Motor_Target = - this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    // mg513_gmr500ppr_motor[3].target.Speed_Motor_Target = - this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);

    /*  X型:   */
    mg513_gmr500ppr_motor[0].target.Speed_Motor_Target =(   this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing)) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    mg513_gmr500ppr_motor[1].target.Speed_Motor_Target =(   this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing)) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    mg513_gmr500ppr_motor[2].target.Speed_Motor_Target =( - this->oriChassis.Speed.vy + this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing)) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);
    mg513_gmr500ppr_motor[3].target.Speed_Motor_Target =( - this->oriChassis.Speed.vy - this->oriChassis.Speed.vx + this->oriChassis.Speed.vw * (Wheel_Spacing + Alex_Spacing)) * 60.0 / (2 * 3.14159265358979f * Wheel_Radius);

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



