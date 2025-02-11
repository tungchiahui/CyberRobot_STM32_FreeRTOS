#include "odom_mg513.h"
#include "mg513_gmr500ppr.h"
#include <cmath>

ODOM_Motor odom_motor_;

// 坐标系满足右手坐标系


//（单位：米）
//轮距
extern fp32 Wheel_Spacing;
//轴距
extern fp32 Alex_Spacing;
//轮子半径
extern fp32 Wheel_Radius;


void ODOM_Motor::Analysis(fp32 dt)
{
	this->dt = dt;
	for(int16_t i = 0;i < 4;i++)
    {
        // 将 RPM 转换为 m/s
			encoder_wheel_velocities_[i] = mg513_gmr500ppr_motor[i].encoder.motor_data.motor_speed  * 2.0f * 3.14159265358979f * Wheel_Radius / 60.0f;
    }

    /* 针对于frame_id（odom） */

    // 计算机器人前进的线速度和角速度，公式不需要轮半径
    //线速度，四个轮子在机器人的运动学模型中贡献相同，所以要除以4
    this->frame_id.vx = ( encoder_wheel_velocities_[0] + encoder_wheel_velocities_[1] - encoder_wheel_velocities_[2] - encoder_wheel_velocities_[3]) / 4.0f;
    this->frame_id.vy = ( encoder_wheel_velocities_[0] - encoder_wheel_velocities_[1] - encoder_wheel_velocities_[2] + encoder_wheel_velocities_[3]) / 4.0f;

    //线速度，轮距（wheel_spacing） 决定了左右轮对旋转的贡献程度，轴距（alex_spacing） 决定了前后轮对旋转的贡献程度，
    //所以要除以底盘尺寸，alex_spacing + wheel_spacing 是底盘尺寸。
    this->frame_id.vw = (-encoder_wheel_velocities_[0] - encoder_wheel_velocities_[1] - encoder_wheel_velocities_[2] - encoder_wheel_velocities_[3]) / (4.0f * (Wheel_Spacing + Alex_Spacing));

    this->frame_id.yaw += this->frame_id.vw * this->dt;
//		this->frame_id.yaw_deg = this->yaw *  180.0f / 3.14159265358979f;

    // 保证 yaw 始终在 -PI 到 PI 之间
    if(this->frame_id.yaw > 3.14159265358979f) 
		{
			this->frame_id.yaw -= 2.0 * 3.14159265358979f;
		}
    if(this->frame_id.yaw < -3.14159265358979f) 
		{
			this->frame_id.yaw += 2.0 * 3.14159265358979f;
		}

    this->frame_id.x_position += (this->frame_id.vx * std::cos(this->frame_id.yaw) - this->frame_id.vy * std::sin(this->frame_id.yaw)) * this->dt;  
    this->frame_id.y_position += (this->frame_id.vx * std::sin(this->frame_id.yaw) + this->frame_id.vy * std::cos(this->frame_id.yaw)) * this->dt;

    /* 针对于child_frame_id(base_link) */

    // 计算相对于child_frame_id(base_link)坐标系的角度
    fp32 cos_yaw = std::cos(this->frame_id.yaw);
    fp32 sin_yaw = std::sin(this->frame_id.yaw);

    // 计算转换后的线速度
    this->child_frame_id.vx =   cos_yaw * this->frame_id.vx + sin_yaw * this->frame_id.vy;
    this->child_frame_id.vy = - sin_yaw * this->frame_id.vx + cos_yaw * this->frame_id.vy;
    // 角速度vw通常在child_frame_id下就不需要转换，保持不变
    this->child_frame_id.vw = this->frame_id.vw;

    this->child_frame_id.yaw = 0.0f;

    // 更新机器人的位置（假设机器人沿着y轴移动）
    // this->child_frame_id.x_position += this->child_frame_id.vx * dt;
    // this->child_frame_id.y_position += this->child_frame_id.vy * dt;
    
    this->child_frame_id.x_position = 0.0f;  
    this->child_frame_id.y_position = 0.0f;
    
}
