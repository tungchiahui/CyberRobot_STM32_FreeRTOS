#include "mg513_gmr500ppr.h"
#include <cmath>
#include <cstdlib>
#include "stm32f407xx.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"
#include "pid_user.h"


MG513_GMR500PPR mg513_gmr500ppr_motor[4];


#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


extern "C"
void StartDefaultTask(void const * argument)
{
    //初始化编码器
    mg513_gmr500ppr_motor[0].encoder.Init(&motor0_encoder_htim);
    mg513_gmr500ppr_motor[1].encoder.Init(&motor1_encoder_htim);
    mg513_gmr500ppr_motor[2].encoder.Init(&motor2_encoder_htim);
    mg513_gmr500ppr_motor[3].encoder.Init(&motor3_encoder_htim);

    //初始化电机驱动器
    mg513_gmr500ppr_motor[0].at8236_cmd.Init(&motor0_pwma_htim,MOTOR0_PWMA_TIM_Channel,&motor0_pwmb_htim,MOTOR0_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[1].at8236_cmd.Init(&motor1_pwma_htim,MOTOR1_PWMA_TIM_Channel,&motor1_pwmb_htim,MOTOR1_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[2].at8236_cmd.Init(&motor2_pwma_htim,MOTOR2_PWMA_TIM_Channel,&motor2_pwmb_htim,MOTOR2_PWMB_TIM_Channel);
    mg513_gmr500ppr_motor[3].at8236_cmd.Init(&motor3_pwma_htim,MOTOR3_PWMA_TIM_Channel,&motor3_pwmb_htim,MOTOR3_PWMB_TIM_Channel);


    //初始化PID控制器
    pid_controller.All_Device_Init();

    //定时器中断
    HAL_TIM_Base_Start_IT(&htim6);

    for(;;)
    {
       mg513_gmr500ppr_motor[0].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.Velocity_Realize(-100,0));
        // mg513_gmr500ppr_motor[0].at8236_cmd.PWM_Pulse_CMD(3000);
//			mg513_gmr500ppr_motor[0].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.VP_Dual_Loop_Realize(-6000,0));

       mg513_gmr500ppr_motor[1].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.Velocity_Realize(-100,1));
        // mg513_gmr500ppr_motor[1].at8236_cmd.PWM_Pulse_CMD(3000);
//			mg513_gmr500ppr_motor[1].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.VP_Dual_Loop_Realize(-6000,1));
//
       mg513_gmr500ppr_motor[2].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.Velocity_Realize(-100,2));
        // mg513_gmr500ppr_motor[2].at8236_cmd.PWM_Pulse_CMD(3000);
//			mg513_gmr500ppr_motor[2].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.VP_Dual_Loop_Realize(-6000,2));

       mg513_gmr500ppr_motor[3].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.Velocity_Realize(-100,3));
        // mg513_gmr500ppr_motor[3].at8236_cmd.PWM_Pulse_CMD(3000);
//			mg513_gmr500ppr_motor[3].at8236_cmd.PWM_Pulse_CMD(pid_controller.motor.VP_Dual_Loop_Realize(-6000,3));
        osDelay(1);
    }
}



extern "C"
/**
 * @brief       定时器更新中断回调函数
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* FreeRTOS的时基，需要注释掉main.c中的HAL_TIM_PeriodElapsedCallback函数 */
    if (htim->Instance == TIM7) 
    {
    HAL_IncTick();
    }

    /* 电机编码器 */
		//电机0
	if(htim->Instance == MOTOR0_ENCODER_TIM_BASE)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&motor0_encoder_htim) == 1)                   /* 判断CR1的DIR位 */
		{
			mg513_gmr500ppr_motor[0].encoder.motor_encoder_count--;                           /* DIR位为1，也就是递减计数 */
		}
		else
		{
			mg513_gmr500ppr_motor[0].encoder.motor_encoder_count++;                          /* DIR位为0，也就是递增计数 */
		}
	}
		//电机1
		if(htim->Instance == MOTOR1_ENCODER_TIM_BASE)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&motor1_encoder_htim) == 1)                   /* 判断CR1的DIR位 */
		{
			mg513_gmr500ppr_motor[1].encoder.motor_encoder_count--;                           /* DIR位为1，也就是递减计数 */
		}
		else
		{
			mg513_gmr500ppr_motor[1].encoder.motor_encoder_count++;                          /* DIR位为0，也就是递增计数 */
		}
	}
	
		//电机2
		if(htim->Instance == MOTOR2_ENCODER_TIM_BASE)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&motor2_encoder_htim) == 1)                   /* 判断CR1的DIR位 */
		{
			mg513_gmr500ppr_motor[2].encoder.motor_encoder_count--;                           /* DIR位为1，也就是递减计数 */
		}
		else
		{
			mg513_gmr500ppr_motor[2].encoder.motor_encoder_count++;                          /* DIR位为0，也就是递增计数 */
		}
	}
	
		//电机3
		if(htim->Instance == MOTOR3_ENCODER_TIM_BASE)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&motor3_encoder_htim) == 1)                   /* 判断CR1的DIR位 */
		{
			mg513_gmr500ppr_motor[3].encoder.motor_encoder_count--;                           /* DIR位为1，也就是递减计数 */
		}
		else
		{
			mg513_gmr500ppr_motor[3].encoder.motor_encoder_count++;                          /* DIR位为0，也就是递增计数 */
		}
	}

    /* 电机编码器运行中断 */
	if(htim->Instance == TIM6)
	{
	    mg513_gmr500ppr_motor[0].encoder.get_finall_encoder_value(&motor0_encoder_htim);
	    mg513_gmr500ppr_motor[1].encoder.get_finall_encoder_value(&motor1_encoder_htim);
	    mg513_gmr500ppr_motor[2].encoder.get_finall_encoder_value(&motor2_encoder_htim);
	    mg513_gmr500ppr_motor[3].encoder.get_finall_encoder_value(&motor3_encoder_htim);
	}
}



/**
 * @brief       AT8236驱动初始化
 * @param        htim_a:定时器句柄指针
 * @param        htim_b:定时器句柄指针
 * @param        TIM_Channel_a:定时器通道
 * @param        TIM_Channel_b:定时器通道
 * @param        maxpulse:最大pulse
 * @note        无
 * @retval      无
 */
void MG513_GMR500PPR::AT8236_Cmd::Init(TIM_HandleTypeDef *htim_a,uint32_t TIM_Channel_a,TIM_HandleTypeDef *htim_b,uint32_t TIM_Channel_b)
{
    this->htim_pwma = htim_a;
    this->TIM_Channel_Pwma = TIM_Channel_a;
    this->htim_pwmb = htim_b;
    this->TIM_Channel_Pwmb = TIM_Channel_b;

    if(htim_a->Init.Period >= htim_b->Init.Period)
    {
        this->max_pulse = htim_a->Init.Period;  //最大重装载数
    }
    if(htim_a->Init.Period < htim_b->Init.Period)
    {
        this->max_pulse = htim_b->Init.Period;  //最大重装载数
    }

    HAL_TIM_PWM_Start(htim_a,TIM_Channel_a);
    HAL_TIM_PWM_Start(htim_b,TIM_Channel_b);

}


/**
 * @brief       AT8236电机驱动控制
 * @param       无
 * @retval      编码器值
 */
void MG513_GMR500PPR::AT8236_Cmd::PWM_Pulse_CMD(int inv_pulse)
{
    int32_t abs_pulse = abs(inv_pulse);
    LimitMax(abs_pulse,this->max_pulse);
	int32_t pulse = this->max_pulse - abs_pulse;

//慢衰减
//转轴方向顺时针旋转为正
	  if(inv_pulse > 0)   //正转
    {
			  __HAL_TIM_SetCompare(this->htim_pwma,this->TIM_Channel_Pwma,this->max_pulse);
        __HAL_TIM_SetCompare(this->htim_pwmb,this->TIM_Channel_Pwmb,pulse);
    }
    else if(inv_pulse < 0)   //反转
    {
        __HAL_TIM_SetCompare(this->htim_pwma,this->TIM_Channel_Pwma,pulse);
        __HAL_TIM_SetCompare(this->htim_pwmb,this->TIM_Channel_Pwmb,this->max_pulse);
    }
    else  //制动
    {
        __HAL_TIM_SetCompare(this->htim_pwma,this->TIM_Channel_Pwma,this->max_pulse);
        __HAL_TIM_SetCompare(this->htim_pwmb,this->TIM_Channel_Pwmb,this->max_pulse);
    }
    
}


void MG513_GMR500PPR::Encoder::Init(TIM_HandleTypeDef *htim)
{
    this->htim_encoder = htim;

    HAL_TIM_Encoder_Start_IT(htim_encoder,TIM_CHANNEL_1);
	  HAL_TIM_Encoder_Start_IT(htim_encoder,TIM_CHANNEL_2);

}


void MG513_GMR500PPR::Encoder::get_finall_encoder_value(TIM_HandleTypeDef *htim)
{
    encoder_data.encoder_now = get_encoder_value(htim);                             /* 获取编码器值，用于计算速度 */
    motor_message_filtering(&encoder_data,&motor_data,LPF_Q,GAP_TIME_MS);      /* 中位平均值滤除编码器抖动数据，50ms计算一次速度*/
}


/**
 * @brief       获取编码器的值
 * @param       无
 * @retval      编码器值
 */
int32_t MG513_GMR500PPR::Encoder::get_encoder_value(TIM_HandleTypeDef *htim)
{
	int32_t encoder_value;
	encoder_value = __HAL_TIM_GET_COUNTER(htim) + motor_encoder_count * 65536;       /* 当前计数值+之前累计编码器的值=总的编码器值 */
	
	return encoder_value;
}


/**
 * @brief       电机速度计算
 * @param       encode_now：当前编码器总的计数值
 *              ms：计算速度的间隔，中断1ms进入一次，例如ms = 5即5ms计算一次速度
 *              q: 低通滤波系数
 * @retval      无
 * @attention   
 */
void MG513_GMR500PPR::Encoder::motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,fp32 q,uint8_t ms)
{
	   //冒泡排序法
    uint8_t i = 0, j = 0;
//	static uint8_t bubble_sort_number = 0;
		//缓存，充当冒泡排序法的交换缓存和冒泡排序法处理后的速度总和缓存
    fp32 temp = 0.0f;
	
//    static uint8_t time_count = 0;
//    static fp32 speed_arr[10] = {0.0f};                     /* 存储速度进行滤波运算 */

    if (this->time_count == ms)                                     /* 计算一次速度 */
    {
        /* 计算电机转速 
           第一步 ：计算ms毫秒内计数变化量
           第二步 ；计算1min内计数变化量：g_encode.speed * ((1000 / ms) * 60 ，
           第三步 ：除以编码器旋转一圈的计数次数（倍频倍数 * 编码器分辨率）
           第四步 ：除以减速比即可得出电机转速
        */
			
        encoder->encoder_delta = (encoder->encoder_now - encoder->encoder_pre);    /* 计算编码器计数值的变化量 */
			if(ABS(encoder->encoder_delta) >= 60000.0f)
			{
				encoder->encoder_delta = 0.0f;
			}
        encoder->encoder_delta_sum += encoder->encoder_delta;
        this->speed_arr[this->bubble_sort_number++] = (fp32)(encoder->encoder_delta * ((1000 / ms) * 60.0) / (FRE_DOU_RATIO) / (PULSE_PER_REVOLUTION));    /* 保存电机转速 */
        
        encoder->encoder_pre = encoder->encoder_now;          /* 保存当前编码器的值 */

        /* 累计10次速度值，后续进行滤波*/
        if (this->bubble_sort_number == 10)
        {
            for (i = 10; i >= 1; i--)                       /* 冒泡排序*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (this->speed_arr[j] > this->speed_arr[j + 1])    /* 数值比较 */
                    { 
                        temp = this->speed_arr[j];                /* 数值换位 */
                        this->speed_arr[j] = this->speed_arr[j + 1];
                        this->speed_arr[j + 1] = temp;
                    }
                }
            }
						
            temp = 0.0;
            
            for (i = 2; i < 8; i++)                         /* 去除两边高低数据 */
            {
                temp += this->speed_arr[i];                       /* 将中间数值累加 */
            }
            
            temp = (fp32)(temp / 6);                       /*求速度平均值*/
            
            /* 一阶低通滤波
             * 公式为：Y(n)= qX(n) + (1-q)Y(n-1)
             * 其中X(n)为本次采样值；Y(n-1)为上次滤波输出值；Y(n)为本次滤波输出值，q为滤波系数
             * q值越小则上一次输出对本次输出影响越大，整体曲线越平稳，但是对于速度变化的响应也会越慢
             */
            motor->rotor_speed = (fp32)((fp32)(q * temp) + (motor->rotor_speed * (fp32)(1.0f-q)) );  //通过低通滤波计算转子速度
						motor->motor_speed = motor->rotor_speed / (REDUCTION_RATIO);     //计算减速后电机转轴的速度
						
						motor->rotor_position =  encoder->encoder_delta_sum;//电机位置
						motor->motor_position =  motor->rotor_position / (REDUCTION_RATIO);  //计算减速后电机转轴的位置
						
						motor->rotor_round_cnt = motor->rotor_position / (FRE_DOU_RATIO) / (PULSE_PER_REVOLUTION);
						motor->motor_round_cnt = motor->rotor_round_cnt / (REDUCTION_RATIO);  //计算减速后电机转轴的圈数
							
            this->bubble_sort_number = 0;
        }
        this->time_count = 0;
    }
    this->time_count ++;
}
