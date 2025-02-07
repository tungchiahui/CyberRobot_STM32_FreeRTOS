#include "odom_mg513.h"
#include "mg513_gmr500ppr.h"
//#include <cmath>
#include "arm_math.h"

ODOM_Motor odom_motor_;


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
    // 计算机器人前进的线速度和角速度，公式不需要轮半径
    //线速度，四个轮子在机器人的运动学模型中贡献相同，所以要除以4
    this->vx = ( encoder_wheel_velocities_[0] + encoder_wheel_velocities_[1] - encoder_wheel_velocities_[2] - encoder_wheel_velocities_[3]) / 4.0f;
    this->vy = (-encoder_wheel_velocities_[0] + encoder_wheel_velocities_[1] + encoder_wheel_velocities_[2] - encoder_wheel_velocities_[3]) / 4.0f;

    //线速度，轮距（wheel_spacing） 决定了左右轮对旋转的贡献程度，轴距（alex_spacing） 决定了前后轮对旋转的贡献程度，
    //所以要除以底盘尺寸，alex_spacing + wheel_spacing 是底盘尺寸。
    this->vw = (encoder_wheel_velocities_[0] + encoder_wheel_velocities_[1] + encoder_wheel_velocities_[2] + encoder_wheel_velocities_[3]) / (4.0f * (Wheel_Spacing + Alex_Spacing));

    // 更新机器人的位置（假设机器人沿着y轴移动）
    // this->x_position += this->vx * std::__math::cos(this->yaw) * this->dt;  
    // this->y_position += this->vy * std::__math::sin(this->yaw) * this->dt;
    this->x_position += this->vx * arm_cos_f32(this->yaw) * this->dt;  
    this->y_position += this->vy * arm_sin_f32(this->yaw) * this->dt;
    this->y_position = - y_position;
    this->yaw += this->vw * this->dt;

    // 保证 yaw 始终在 -PI 到 PI 之间
    if (this->yaw > 3.14159265358979f) this->yaw -= 2.0 * 3.14159265358979f;
    if (this->yaw < -3.14159265358979f) this->yaw += 2.0 * 3.14159265358979f;
    
    this->yaw = - this->yaw;
    
}
