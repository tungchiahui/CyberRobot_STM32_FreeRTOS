#ifndef __MG513_GMR500PPR_H_
#define __MG513_GMR500PPR_H_

#include <cstdint>
#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
#include "tim.h"

#define ABS(x)	( (x>0) ? (x) : (-x) ) 

#define PULSE_PER_REVOLUTION  500.0f	/* 线数 */
#define FRE_DOU_RATIO         4.0f  	/* 倍频系数，根据编码器模式选 */
#define REDUCTION_RATIO       30.0f  /* 减速比x:1 */
#define GAP_TIME_MS           1       //采集间隔时长
#define LPF_Q                 0.9f    //低通滤波Q系数

//速度范围:0-900rpm
//单圈角度范围:0-2000
	
	
//Initialize motor speed control and, for controlling motor speed, PWM frequency 10kHz
//初始化电机速度控制以及，用于控制电机速度，PWM频率10KHZ
//APB2时钟频率为168M，满PWM为16799，频率=168M/((16799+1)*(0+1))=10k
//PSC = 1-1
//ARR = 16800-1
	
	
//电机驱动板AT8236对应的定时器
//TIM1  APB2
//TIM9  APB2
//TIM10 APB2
//TIM11 APB2
#define MOTOR0_PWMA_TIM_BASE      	 TIM10
#define MOTOR0_PWMA_TIM_Channel    	 TIM_CHANNEL_1
#define motor0_pwma_htim    		 htim10

#define MOTOR0_PWMB_TIM_BASE      	 TIM11
#define MOTOR0_PWMB_TIM_Channel    	 TIM_CHANNEL_1
#define motor0_pwmb_htim    		 htim11


#define MOTOR1_PWMA_TIM_BASE      	 TIM9
#define MOTOR1_PWMA_TIM_Channel    	 TIM_CHANNEL_1
#define motor1_pwma_htim    		 htim9

#define MOTOR1_PWMB_TIM_BASE      	 TIM9
#define MOTOR1_PWMB_TIM_Channel    	 TIM_CHANNEL_2
#define motor1_pwmb_htim    		 htim9


#define MOTOR2_PWMA_TIM_BASE      	 TIM1
#define MOTOR2_PWMA_TIM_Channel    	 TIM_CHANNEL_1
#define motor2_pwma_htim    		 htim1

#define MOTOR2_PWMB_TIM_BASE      	 TIM1
#define MOTOR2_PWMB_TIM_Channel    	 TIM_CHANNEL_2
#define motor2_pwmb_htim    		 htim1


#define MOTOR3_PWMA_TIM_BASE      	 TIM1
#define MOTOR3_PWMA_TIM_Channel    	 TIM_CHANNEL_3
#define motor3_pwma_htim    		 htim1

#define MOTOR3_PWMB_TIM_BASE      	 TIM1
#define MOTOR3_PWMB_TIM_Channel    	 TIM_CHANNEL_4
#define motor3_pwmb_htim    		 htim1


//电机编码器对应的定时器
//TIM2 APB1
//TIM3 APB1
//TIM4 APB1
//TIM5 APB1
//TIM6 APB1
#define MOTOR0_ENCODER_TIM_BASE      TIM2
#define motor0_encoder_htim    		 htim2

#define MOTOR1_ENCODER_TIM_BASE      TIM3
#define motor1_encoder_htim    		 htim3

#define MOTOR2_ENCODER_TIM_BASE      TIM4
#define motor2_encoder_htim    		 htim4

#define MOTOR3_ENCODER_TIM_BASE      TIM5
#define motor3_encoder_htim    		 htim5




typedef struct
{
	int32_t encoder_now;
	int32_t encoder_pre;
	int32_t encoder_delta;
	int32_t encoder_delta_sum;
}Encoder_TypeDef;


/* 电机参数结构体 */
typedef struct 
{
  bool_t state;          /*电机状态*/
  fp32 current;          /*电机电流*/
  fp32 volatage;         /*电机电压*/
  fp32 power;            /*电机功率*/
  fp32 rotor_speed;      /*电机[转子]实际速度*/
	fp32 motor_speed;		   /*电机[转轴]实际速度*/
	fp32 rotor_position;	 /*电机[转子]实际转动距离*/
	fp32 motor_position;	 /*电机[转轴]实际转动距离*/
	fp32 rotor_round_cnt;  /*电机[转子]实际转动圈数*/
	fp32 motor_round_cnt;  /*电机[转轴]实际转动圈数*/
  int32_t motor_pwm;     /*设置比较值大小 */
}Motor_TypeDef;



class MG513_GMR500PPR
{
	public:

	class AT8236_Cmd
	{
		public:
		TIM_HandleTypeDef *htim_pwma;
		uint32_t TIM_Channel_Pwma;
		TIM_HandleTypeDef *htim_pwmb;
		uint32_t TIM_Channel_Pwmb;
		int max_pulse;

		void Init(TIM_HandleTypeDef *htim_a,uint32_t TIM_Channel_a,TIM_HandleTypeDef *htim_b,uint32_t TIM_Channel_b);
		void PWM_Pulse_CMD(int inv_pulse);
		
	}at8236_cmd;

	class Encoder
	{
		public:
		TIM_HandleTypeDef *htim_encoder;
		
		//motor_message_filtering函数要用到的变量
		uint8_t bubble_sort_number = 0;
		uint8_t time_count = 0;
		fp32 speed_arr[10] = {0.0f};

		volatile int32_t motor_encoder_count;   //溢出次数
		Encoder_TypeDef encoder_data;
		Motor_TypeDef motor_data;

		void Init(TIM_HandleTypeDef *htim);
		int32_t get_encoder_value(TIM_HandleTypeDef *htim);                             									 //获取编码器的值
		void motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,fp32 q,uint8_t ms);   //电机速度计算
		void get_finall_encoder_value(TIM_HandleTypeDef *htim);

	}encoder;

};


#ifdef __cplusplus
}
#endif

#endif
