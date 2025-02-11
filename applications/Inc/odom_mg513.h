#ifndef __ODOM_MG513_H_
#define __ODOM_MG513_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"



class ODOM_Motor
{
    public:
    void Analysis(fp32 dt);

    fp32 encoder_wheel_velocities_[4];

    class FRAME
    {
        public:
            fp32 vx;
            fp32 vy;
            fp32 vw;
            fp32 x_position;
            fp32 y_position;
            fp32 yaw;
        //		fp32 yaw_deg;
    }frame_id;
    class CHILD_FRAME
    {
        public:
            fp32 vx;
            fp32 vy;
            fp32 vw;
            fp32 x_position;
            fp32 y_position;
            fp32 yaw;
        //		fp32 yaw_deg;
    }child_frame_id;


    //Analysis函数触发周期，单位是秒s
    fp32 dt;
};


extern ODOM_Motor odom_motor_;
	

#ifdef __cplusplus
}
#endif
	
#endif
