#include "chassis_task.h"
#include "chassis_api.h"
#include "mg513_gmr500ppr.h"
#include "pid_user.h"


extern "C"
void chassis_task(void const * argument)
{

    // wait a time
	//空闲一段时间
	vTaskDelay(CHASSIS_TASK_INIT_TIME);

    //初始化编码器
    mg513_gmr500ppr_motor[0].encoder.Init(&motor0_encoder_htim);
    mg513_gmr500ppr_motor[1].encoder.Init(&motor1_encoder_htim);
    mg513_gmr500ppr_motor[2].encoder.Init(&motor2_encoder_htim);
    mg513_gmr500ppr_motor[3].encoder.Init(&motor3_encoder_htim);

    //编码器定时器中断
    HAL_TIM_Base_Start_IT(&htim6);

    //初始化电机驱动器
    mg513_gmr500ppr_motor[0].at8236_cmd.Init(&motor0_pwma_htim,MOTOR0_PWMA_TIM_Channel,&motor0_pwmb_htim,MOTOR0_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[1].at8236_cmd.Init(&motor1_pwma_htim,MOTOR1_PWMA_TIM_Channel,&motor1_pwmb_htim,MOTOR1_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[2].at8236_cmd.Init(&motor2_pwma_htim,MOTOR2_PWMA_TIM_Channel,&motor2_pwmb_htim,MOTOR2_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[3].at8236_cmd.Init(&motor3_pwma_htim,MOTOR3_PWMA_TIM_Channel,&motor3_pwmb_htim,MOTOR3_PWMB_TIM_Channel);


    //初始化PID控制器
    pid_controller.All_Device_Init();

    //绝对延时
    uint32_t PreviousWakeTime = osKernelSysTick();

    for(;;)
    {
        //set chassis motion mode
		//设定底盘运动模式
        chassis.Remote_Control_Chassis_Set_Mode();

        //chassis data update
		//底盘数据更新
        chassis.Remote_Control_Chassis_Mode();
        chassis.Chassis_Orientation_Analysis();

		//calculate the speed of the chassis
		//底盘速度解算
        chassis.Chassis_Kinematics_Calc();

		//chassis control pid calculate
		//底盘控制PID计算与数据发送
        chassis.Chassis_Loop_Out();

        //chassis task control time
		//底盘任务控制间隔
		osDelayUntil(&PreviousWakeTime,CHASSIS_CONTROL_TIME_MS);
    }
}

