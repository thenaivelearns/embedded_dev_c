#include "Control_Task.h"
#include "control.h"
#include "led.h"

ControlMsg_t control_data;

float front_back_rocker = 0.0;                        //左边 - 前后摇杆位置
float left_right_rocker = 0.0;                        //左边 - 左右摇杆位置
float up_down_rocker = 0.0;                           //右边 - 上下摇杆位置
float left_right_adjust_rock = 0.0;                   //右边 - 左右摇杆位置

int16_t test_motor = 45;//初始左右运动转速
float heading_pid_tar = 0;  
float heading_pid_out = 0;
//float pitchrange=50;//俯仰角的调整变化范围
//float depthrange=70;//深度调节变化范围
float pitching_pid_out = 0; 
float deepth_pid_out = 0;  
float deep_tar = 30;//目标深度
float navi_position  = 0.0;
int a=0;
int b=0;
char stringstore2[50]={0};
char stringstore3[50]={0};
char stringstore4[50]={0};
extern void LED_SW(u8 num,u8 sta);
float W0=0.7;float W1=1;//参数的比重，分别是距离和角度
float headdis,headang;//设置距离和角度的偏置值
float disdead=10;
float angdead=1;
int mod=0;  //模式，0为正常前进，1为识别小球悬停，2位转转向，3为识别字母悬停
extern PID_t findline_pid;
extern int k;//寻到小球标志位
int max_k=1;
int flag=200;
int turnangle1=-90;int turnangle2=-135;int turnangle3=45;//每次转向角度，左边为正
extern char letter1,letter2,letter3,letter4;

void ledlighter(void);//灯亮程序
void leddown(void);//灯灭
void turndirection(int);//转向角度
void updowndepth(void);//定身俯仰角程序
void senddata(void);//传送数据程序
void sendletter(void);

//俯仰使能
u8 pitch_en =1;
u8 deep_en = 1;
void control_Task_Function(void const * argument)
{
 
  portTickType tick = xTaskGetTickCount();
	Stm32_Pi_Init();
	while(1)
	{ 
		if(xQueueReceive(Control_Message_Queue,&(control_data),portMAX_DELAY)) 
		{
			//俯仰角 PID 调节
//			if(pitch_en==1||deep_en==1){
//				pitching_pid_out = Incremental_PID_Cal(0,control_data.rolling,&Pitch_pid);
//				pitching_pid_out =CONSTRAIN(pitching_pid_out,pitchrange,(-1)*pitchrange);
			//深度调节				
	//			deepth_pid_out = Incremental_PID_Cal(deep_tar,control_data.depth,&Deep_pid);	
//				deepth_pid_out =CONSTRAIN(deepth_pid_out,depthrange,(-1)*depthrange);
	//			SetUpDowm((-1)*0.5*pitching_pid_out+deepth_pid_out,0.5*pitching_pid_out+deepth_pid_out);		
	//	}else{
	//		SetUpDowm(0,0);		
	//		}
			//定身和俯仰角控制
  if(pitch_en==1||deep_en==1)
			updowndepth();
			else{
			SetUpDowm(0,0);}
////////////////////////////////
			//巡线运动
		if(control_data.Control_Disable==true){
			if(control_data.navi_distance>disdead||control_data.navi_distance<(-disdead))
			{	headdis=Incremental_PID_Cal(0,control_data.navi_distance/200,&findline_pid);
				headdis =CONSTRAIN(headdis,10,-10);}
			else{headdis=0;}
			if(control_data.navi_angle>angdead||control_data.navi_angle<(-angdead))
			{	headang=Incremental_PID_Cal(0,control_data.navi_angle/60,&findline_pid);
				headang =CONSTRAIN(headang,10,-10);}
			else{headang=0;}
			navi_position=W0*headdis+(-1)*W1*headang;
			SetHorizontal(test_motor,(-1)*navi_position);}
		else{SetHorizontal(0,0);}
			}
		////////////////////////////
			//寻到小球判断,然后悬停
			flag--;
			if(k<0&&flag<=0)
			{
				flag=200;
	//			if(k==1){
				k=2;
				ledlighter();
				mod=1;//}
				
			}//	
			/////////////////
			//悬停，不是第一次转向
			if(mod==1)
			{	//sprintf(stringstore2,"已寻到小球，现悬停\r\n");
				//SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
				///////////////////
				for(int i=1;i<=400;i++)
				{
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//左右螺旋桨关闭
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
					updowndepth();
					delay_ms(5);}
				//这里写悬停程序
				leddown();//关灯
				mod=2;
				b++;
			}
//			///////////////
//				if(b==1)//第一次直接前行
//				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(-1)*test_motor+right_mid-4-15);
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_motor+left_mid+4+25);
//					delay_ms(500);
					
//      if(control_data.Control_Disable==true)
//				{
//			if(control_data.navi_distance>disdead||control_data.navi_distance<(-disdead))
//			{	headdis=Incremental_PID_Cal(0,control_data.navi_distance/200,&findline_pid);
//				headdis =CONSTRAIN(headdis,10,-10);}
//			else{headdis=0;}
//			if(control_data.navi_angle>angdead||control_data.navi_angle<(-angdead))
//			{	headang=Incremental_PID_Cal(0,control_data.navi_angle/60,&findline_pid);
//				headang =CONSTRAIN(headang,10,-10);}
//			else{headang=0;}
//			navi_position=W0*headdis+(-1)*W1*headang;
//			SetHorizontal(test_motor,(-1)*navi_position);}
//		else{SetHorizontal(0,0);}
//			}

//				
//			//转向设置
			if(mod==2&&b>=3)
			{///////////
				//先后退
				for(int i=1;i<=10;i++)
				{
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_motor+right_mid+4+25);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(-1)*test_motor+left_mid-4-15);
					updowndepth();
					delay_ms(100);}
				//看到的是第几个球，第一个不转向
				if(b==3&&b>4)
				turndirection(turnangle1);
				else if(b==4)
				{sendletter();
					turndirection(turnangle2);}
				else 
				{
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1500);
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1500);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1500);
					while(1)
					{}
				}
				//////////////////////////////
				//sprintf(stringstore2,"转向结束，开始继续前行\r\n");
				//SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
				//随缘前行
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(-1)*test_motor+right_mid-4-15);
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_motor+left_mid+4+25);
//					updowndepth();
//				mod=0;
//				HAL_Delay(500);
////				
//				  if(control_data.Control_Disable==true){
//			if(control_data.navi_distance>disdead||control_data.navi_distance<(-disdead))
//			{	headdis=Incremental_PID_Cal(0,control_data.navi_distance/200,&findline_pid);
//				headdis =CONSTRAIN(headdis,10,-10);}
//			else{headdis=0;}
//			if(control_data.navi_angle>angdead||control_data.navi_angle<(-angdead))
//			{	headang=Incremental_PID_Cal(0,control_data.navi_angle/60,&findline_pid);
//				headang =CONSTRAIN(headang,10,-10);}
//			else{headang=0;}
//			navi_position=W0*headdis+(-1)*W1*headang;
//			SetHorizontal(test_motor,(-1)*navi_position);}
//		else{SetHorizontal(0,0);}
}
////			}
//			///////////////
//			//检测到字符之后，开始悬停识别
//			if(mod==3)
//			{
//				for(int i=1;i<=1000;i++)
//				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//左右螺旋桨关闭
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
//					updowndepth();
//					delay_ms(5);}
//				
//				sprintf(stringstore2,"寻找到4个字母，正在识别\r\n");
//				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
//				HAL_Delay(500);
//				sprintf(stringstore2,"4个字母分别是%c，%c，%c，%c\r\n",letter1,letter2,letter3,letter4);
//				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
//					
//					for(int i=1;i<=1000;i++)
//				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//左右螺旋桨关闭
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
//					updowndepth();
//					delay_ms(5);}
//			}			
			mod=0;
		//发送数据
//		a++;
//		if(a==1000)
//		{a=0;
//	  senddata();
//		////////////
	
	}
}



void updowndepth(void)//控制定深和俯仰角
{			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back_mid+81);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front_mid-71);//	pitching_pid_out = Incremental_PID_Cal(0,control_data.rolling,&Pitch_pid);//控制俯仰角
//				pitching_pid_out =CONSTRAIN(pitching_pid_out,80,-80);
//				SetUpDowm((-1)*pitching_pid_out,pitching_pid_out);
//			deepth_pid_out = Incremental_PID_Cal(deep_tar,control_data.depth,&Deep_pid);	//控制深度
//			deepth_pid_out =CONSTRAIN(deepth_pid_out,95,-95);
//     SetUpDowm(deepth_pid_out,deepth_pid_out);
//	if((deep_tar-control_data.depth)>=20||(deep_tar-control_data.depth)<=-20)
//	{if((deep_tar-control_data.depth)>=20){
		
//}
//	else{
//			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back_mid-115);
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front_mid+115);}
//}
}
	
void ledlighter(void)//灯亮程序
{	for (int i=1;i<=8;i++)
	LED_SW(i,1);
}
void leddown(void)//灯灭程序
{
	for (int i=1;i<=8;i++)
	LED_SW(i,0);
}

void turndirection(int turnangle)//角度转向
{	
	
	if(turnangle>0)
	{int c=(int)(turnangle*1.25);
			for(int i=1;i<=c;i++)
				{	
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_motor+right_mid+4+20);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_motor+left_mid+4+20);
				updowndepth();
					HAL_Delay(5);
				}
	}
	else
	{int c=(int)(turnangle*3.5);
			for(int i=1;i<=(-c);i++)
				{	
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(-1)*test_motor+right_mid-4-20);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(-1)*test_motor+left_mid-4-20);
						updowndepth();
					HAL_Delay(5);
				}
	}
		
}
void senddata(void){//传输数据
	sprintf(stringstore2,"深度=%.2fcm\r\n",control_data.depth);
		SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
			 sprintf(stringstore2,"俯仰角=%.2f度\r\n",control_data.depth);
		SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
		sprintf(stringstore3,"距离=%.2fcm\r\n",control_data.navi_distance);
		SX1278_SendData((uint8_t*)stringstore3,strlen(stringstore3));
			sprintf(stringstore3,"角度 = %.2f度\r\n",control_data.navi_angle);
		SX1278_SendData((uint8_t*)stringstore3,strlen(stringstore3));
		sprintf(stringstore4,"\r\n");
		SX1278_SendData((uint8_t*)stringstore4,strlen(stringstore4));
}
void sendletter(void)
{
	sprintf(stringstore2,"寻找到4个字母，正在识别\r\n");
				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
	for(int i=1;i<=400;i++)
				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//左右螺旋桨关闭
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
					updowndepth();
					delay_ms(5);}
				sprintf(stringstore2,"4个字母分别是%c，%c，%c，%c\r\n",letter1,letter2,letter3,letter4);
				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
}