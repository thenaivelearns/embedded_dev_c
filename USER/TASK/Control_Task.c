#include "Control_Task.h"
#include "control.h"
#include "led.h"

ControlMsg_t control_data;

float front_back_rocker = 0.0;                        //��� - ǰ��ҡ��λ��
float left_right_rocker = 0.0;                        //��� - ����ҡ��λ��
float up_down_rocker = 0.0;                           //�ұ� - ����ҡ��λ��
float left_right_adjust_rock = 0.0;                   //�ұ� - ����ҡ��λ��

int16_t test_motor = 45;//��ʼ�����˶�ת��
float heading_pid_tar = 0;  
float heading_pid_out = 0;
//float pitchrange=50;//�����ǵĵ����仯��Χ
//float depthrange=70;//��ȵ��ڱ仯��Χ
float pitching_pid_out = 0; 
float deepth_pid_out = 0;  
float deep_tar = 30;//Ŀ�����
float navi_position  = 0.0;
int a=0;
int b=0;
char stringstore2[50]={0};
char stringstore3[50]={0};
char stringstore4[50]={0};
extern void LED_SW(u8 num,u8 sta);
float W0=0.7;float W1=1;//�����ı��أ��ֱ��Ǿ���ͽǶ�
float headdis,headang;//���þ���ͽǶȵ�ƫ��ֵ
float disdead=10;
float angdead=1;
int mod=0;  //ģʽ��0Ϊ����ǰ����1Ϊʶ��С����ͣ��2λתת��3Ϊʶ����ĸ��ͣ
extern PID_t findline_pid;
extern int k;//Ѱ��С���־λ
int max_k=1;
int flag=200;
int turnangle1=-90;int turnangle2=-135;int turnangle3=45;//ÿ��ת��Ƕȣ����Ϊ��
extern char letter1,letter2,letter3,letter4;

void ledlighter(void);//��������
void leddown(void);//����
void turndirection(int);//ת��Ƕ�
void updowndepth(void);//�������ǳ���
void senddata(void);//�������ݳ���
void sendletter(void);

//����ʹ��
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
			//������ PID ����
//			if(pitch_en==1||deep_en==1){
//				pitching_pid_out = Incremental_PID_Cal(0,control_data.rolling,&Pitch_pid);
//				pitching_pid_out =CONSTRAIN(pitching_pid_out,pitchrange,(-1)*pitchrange);
			//��ȵ���				
	//			deepth_pid_out = Incremental_PID_Cal(deep_tar,control_data.depth,&Deep_pid);	
//				deepth_pid_out =CONSTRAIN(deepth_pid_out,depthrange,(-1)*depthrange);
	//			SetUpDowm((-1)*0.5*pitching_pid_out+deepth_pid_out,0.5*pitching_pid_out+deepth_pid_out);		
	//	}else{
	//		SetUpDowm(0,0);		
	//		}
			//����͸����ǿ���
  if(pitch_en==1||deep_en==1)
			updowndepth();
			else{
			SetUpDowm(0,0);}
////////////////////////////////
			//Ѳ���˶�
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
			//Ѱ��С���ж�,Ȼ����ͣ
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
			//��ͣ�����ǵ�һ��ת��
			if(mod==1)
			{	//sprintf(stringstore2,"��Ѱ��С������ͣ\r\n");
				//SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
				///////////////////
				for(int i=1;i<=400;i++)
				{
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//�����������ر�
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
					updowndepth();
					delay_ms(5);}
				//����д��ͣ����
				leddown();//�ص�
				mod=2;
				b++;
			}
//			///////////////
//				if(b==1)//��һ��ֱ��ǰ��
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
//			//ת������
			if(mod==2&&b>=3)
			{///////////
				//�Ⱥ���
				for(int i=1;i<=10;i++)
				{
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_motor+right_mid+4+25);
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(-1)*test_motor+left_mid-4-15);
					updowndepth();
					delay_ms(100);}
				//�������ǵڼ����򣬵�һ����ת��
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
				//sprintf(stringstore2,"ת���������ʼ����ǰ��\r\n");
				//SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
				//��Եǰ��
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
//			//��⵽�ַ�֮�󣬿�ʼ��ͣʶ��
//			if(mod==3)
//			{
//				for(int i=1;i<=1000;i++)
//				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//�����������ر�
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
//					updowndepth();
//					delay_ms(5);}
//				
//				sprintf(stringstore2,"Ѱ�ҵ�4����ĸ������ʶ��\r\n");
//				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
//				HAL_Delay(500);
//				sprintf(stringstore2,"4����ĸ�ֱ���%c��%c��%c��%c\r\n",letter1,letter2,letter3,letter4);
//				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
//					
//					for(int i=1;i<=1000;i++)
//				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//�����������ر�
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
//					updowndepth();
//					delay_ms(5);}
//			}			
			mod=0;
		//��������
//		a++;
//		if(a==1000)
//		{a=0;
//	  senddata();
//		////////////
	
	}
}



void updowndepth(void)//���ƶ���͸�����
{			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,back_mid+81);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front_mid-71);//	pitching_pid_out = Incremental_PID_Cal(0,control_data.rolling,&Pitch_pid);//���Ƹ�����
//				pitching_pid_out =CONSTRAIN(pitching_pid_out,80,-80);
//				SetUpDowm((-1)*pitching_pid_out,pitching_pid_out);
//			deepth_pid_out = Incremental_PID_Cal(deep_tar,control_data.depth,&Deep_pid);	//�������
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
	
void ledlighter(void)//��������
{	for (int i=1;i<=8;i++)
	LED_SW(i,1);
}
void leddown(void)//�������
{
	for (int i=1;i<=8;i++)
	LED_SW(i,0);
}

void turndirection(int turnangle)//�Ƕ�ת��
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
void senddata(void){//��������
	sprintf(stringstore2,"���=%.2fcm\r\n",control_data.depth);
		SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
			 sprintf(stringstore2,"������=%.2f��\r\n",control_data.depth);
		SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
		sprintf(stringstore3,"����=%.2fcm\r\n",control_data.navi_distance);
		SX1278_SendData((uint8_t*)stringstore3,strlen(stringstore3));
			sprintf(stringstore3,"�Ƕ� = %.2f��\r\n",control_data.navi_angle);
		SX1278_SendData((uint8_t*)stringstore3,strlen(stringstore3));
		sprintf(stringstore4,"\r\n");
		SX1278_SendData((uint8_t*)stringstore4,strlen(stringstore4));
}
void sendletter(void)
{
	sprintf(stringstore2,"Ѱ�ҵ�4����ĸ������ʶ��\r\n");
				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
	for(int i=1;i<=400;i++)
				{__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,left_mid);//�����������ر�
					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,right_mid);
					updowndepth();
					delay_ms(5);}
				sprintf(stringstore2,"4����ĸ�ֱ���%c��%c��%c��%c\r\n",letter1,letter2,letter3,letter4);
				SX1278_SendData((uint8_t*)stringstore2,strlen(stringstore2));
}