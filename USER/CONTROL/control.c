#include "control.h"

//垂直方向电机 速度设置
void SetUpDowm(float front_val,float back_val)//min--max 升降运动
{//下降的时候都于0
	if(front_val>0)
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front_mid+front_val+10);
	else
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front_mid+front_val);
	if(back_val>0)
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back_mid-back_val);
	else
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back_mid-back_val+10);
//	//3 // 电机三根线反了
//	 static 	uint16_t	last_value_front = mid;
//	 static 	uint16_t	last_value_back = mid;
// 
//    uint16_t front = mid + front_val;
//    uint16_t back = mid - back_val;  // 电机三根线反了
//    
//  front = CONSTRAIN(front, last_value_front+max_speed_delta, last_value_front-max_speed_delta);//#define CONSTRAIN(x,max,min) (x>max?max:(x<min?min:x))
//    back = CONSTRAIN(back, last_value_back+max_speed_delta, last_value_back-max_speed_delta);

//    front = CONSTRAIN(front, max_speed_value, min_speed_value);
//    back = CONSTRAIN(back, max_speed_value, min_speed_value);
//	
//		if(front_val>0)
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front+10);
//	else
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front);
//	if(back_val>0)
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back);
//	else
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back+10);
////	
////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front); //1
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,back);//3
//	
//	
//    last_value_front = front;
//    last_value_back = back;
} 

float temp_l = 0;
float temp_r =0;
//水平方向电机 速度设置
void SetHorizontal(float f_b, float rotate)//水平运动
{if(rotate<0)
	{temp_r = (-1)*f_b+right_mid+2;
	temp_l = f_b+left_mid-1.4*rotate+5;}
		else
		{temp_r = (-1)*f_b+right_mid-1.4*rotate;
	temp_l = f_b+left_mid-2;}
		
  //  PWM_Init();
	//控制方案一
 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,temp_r);//2
 	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,temp_l);//4 
	
	
	//控制方案二
//	static uint16_t last_value_left = left_mid;
//  static uint16_t last_value_right = right_mid;
	
//	float right = right_mid +(-1)* f_b - rotate;//右
//	float left = left_mid + f_b + rotate;//左
 
//	right = CONSTRAIN(right, last_value_right+max_speed_delta, last_value_right-max_speed_delta);
//	left = CONSTRAIN(left, last_value_left+max_speed_delta, last_value_left-max_speed_delta);
    
 // left = CONSTRAIN(left, max_speed_value, min_speed_value);
//  right = CONSTRAIN(right, max_speed_value, min_speed_value);

// __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,right);//2
// 	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,left);//4

//	last_value_left =  left;
	//last_value_right = right;

}

u16 xCounterPi_To_Stm32 = 5;
void Clear_Control_Disable_Counter(void)
{
	xCounterPi_To_Stm32 = 0;
}

bool Get_Control_Disable_Counter_State(void)
{
	bool ret;
	if(xCounterPi_To_Stm32 >=5){
		ret = false;
	}else{
		ret = true;
	}
	return ret;
}

void Adder_Control_Disable_Counter(void)
{
	if(xCounterPi_To_Stm32>=5){
		xCounterPi_To_Stm32 = 5;
	}else{
		xCounterPi_To_Stm32++;
	}
}