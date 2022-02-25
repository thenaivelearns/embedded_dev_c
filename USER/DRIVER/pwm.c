#include "pwm.h"
////摄像头使用
//void PWM_ServoInit(void)
//{
////	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,SERVO_INIT);
//}
//int GetAngle = 0;
//void SetCameraAngle(int value)
//{
////	GetAngle = value;
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(GetAngle-50)*6+SERVO_INIT);
//}

uint16_t test_pwm=1500;
uint16_t test_pwm1=1500;
uint16_t test_pwm2=1500;
uint16_t test_pwm3=1500;
uint16_t test_pwm4=1500;
uint16_t test_pwm5=1500;
uint16_t test_pwm6=1500;
uint16_t test_pwm7=1500;
uint16_t test_pwm8=1500;
void PWM_Init(void)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,mid);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,mid);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,right_mid);//  水平  右推进器
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,mid);
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,mid);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,mid);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,left_mid);//  水平  左推进器
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,mid);
	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
}


void PWM_Test_Demo(void)
{
	PWM_Init();
	while(1)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500);//1   上下
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1500);// 
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1457);//2   前后  
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1500);//
		
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1500);//3   上下
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1500);// 
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1554);//4   前后
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1500);

		HAL_Delay(500);
	}
}