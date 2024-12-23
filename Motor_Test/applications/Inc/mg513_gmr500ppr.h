#ifndef __MG513_GMR500PPR_H_
#define __MG513_GMR500PPR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
#include "tim.h"

#define ABS(x)	( (x>0) ? (x) : (-x) ) 

#define PULSE_PER_REVOLUTION  13.0f	/* 线数 */
#define FRE_DOU_RATIO         4.0f  	/* 倍频系数，根据编码器模式选 */
#define REDUCTION_RATIO       30.0f  /* 减速比x:1 */
#define GAP_TIME_MS           50     //采集间隔时长
#define LPF_Q                 0.9f    //低通滤波Q系数


//电机编码器对应的定时器
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
	void Init(void);

	class AT8236_Cmd
	{
		public:
		void PWM_Pulse_CMD(int16_t i,int pulse);
		
	}at8236_cmd;

	class Encoder
	{
		public:
		volatile int32_t motor_encoder_count;   //溢出次数
		Encoder_TypeDef encoder_data;
		Motor_TypeDef motor_data;

		int32_t get_encoder_value(TIM_HandleTypeDef *htim);                             									 //获取编码器的值
		void motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,fp32 q,uint8_t ms);   //电机速度计算
		void get_finall_encoder_value(TIM_HandleTypeDef *htim);

	}encoder;

};


int32_t get_encoder_value(void);
void motor_message_filtering(Encoder_TypeDef *encoder,Motor_TypeDef *motor,fp32 q,uint8_t ms);


#ifdef __cplusplus
}
#endif

#endif
