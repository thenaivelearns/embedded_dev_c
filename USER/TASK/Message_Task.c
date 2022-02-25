#include "Message_Task.h"
#include "stdlib.h"
static ControlMsg_t control_data;
static PowerMsg_t power_data;
static IMUMsg_t imu_data;
static TempHumiMsg_t temp_humi_data;
PressureMsg_t pressure_data;

float navi_center_to_line_angle = 0.0;   //中心到色带角度
float navi_center_to_line_distance = 0.0;//中心到色带距离
int16_t front_speed = 0;
char temp_buf[30]={0};
u32 pack_num = 0;
int k=2;//检测到球的标志位
char letter1,letter2,letter3,letter4;//4个字母
void message_Task_Function(void const * argument)
{
  BaseType_t err;
	portTickType tick = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
		#ifdef DEBUG
		//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);		 
		#else
 
		xQueuePeek(TempHum_Message_Queue,&temp_humi_data,1);//温湿度
		xQueuePeek(IMU_Message_Queue,&imu_data,1);          //电子罗盘
		xQueuePeek(Pressure_Message_Queue,&pressure_data,1);//压传
		xQueuePeek(Battery_Message_Queue,&power_data,1);    //压传
		
		control_data.heading = imu_data.heading;
		control_data.pitching = imu_data.pitch;
		control_data.rolling = imu_data.roll-1.2;
		control_data.depth = pressure_data.depth;
	  control_data.temperture = pressure_data.Temperature;

 
		Adder_Control_Disable_Counter(); 		
		control_data.Control_Disable = Get_Control_Disable_Counter_State();

		if(Control_Message_Queue!=NULL)//发给控制任务 
		{   
				err=xQueueSend(Control_Message_Queue,&control_data,1);	// 手柄控制
				if(err==errQUEUE_FULL)   	 
				{
						;//printf("队列Key_Queue已满，数据发送失败!\r\n");
				}
		}
		
	//	static int time = 0;
		//if(time>10){time = 0;
			//sprintf(temp_buf,"dis=%.2f,ang = %.2f\r\n",control_data.navi_distance,control_data.navi_angle);
		//	SX1278_SendData((uint8_t*)temp_buf,strlen(temp_buf));
		//}else{time ++;}
		#endif
 		vTaskDelayUntil(&tick,100);
	}
}
float p_val = 0.0;
float i_val = 0.0;
float d_val = 0.0;
void SX1278_unpackData(uint8_t *buf,u16 len)
{
	if(strstr((char*)buf,"P:")){
//		p_val = atof(buf);	
	}
	if(strstr((char*)buf,"I:")){
		
	}
  if(strstr((char*)buf,"D:")){
		
	}	 
}
void Pi_Stm32_unpackData(uint8_t *buf,u16 len)
{
		
	u8 step = 0;
	for(int i =0;i<len;i++)
	{
		switch(step)
		{
			case 0:                      //检测包头
				if(buf[i]==0xFE){
					step = 1;}
				break;
			case 1:
				if(buf[i]<=(len-i+1)){
					if(buf[buf[i]-1+(i-1)] == 0x0a){//检测包长
						step = 2;
					}					
				}else{
           step = 0;
				}
				break;
			case 2:i++;
		   switch(buf[i])
        {
            case MSG_NAVI_ID:
                memcpy((void*)&navi_center_to_line_angle,(void*)&(buf[i+1]),4); 
						    memcpy((void*)&navi_center_to_line_distance,(void*)&(buf[i+5]),4);
						 		control_data.navi_distance = navi_center_to_line_distance;
	            	control_data.navi_angle    = navi_center_to_line_angle;
                Clear_Control_Disable_Counter(); 		
                break;
           
						case MSG_yellow_ball:
								k--;
						Clear_Control_Disable_Counter();
						break;
						case MSG_letter:
							 memcpy((void*)&letter1,(void*)&(buf[i+1]),2); 
						    memcpy((void*)&letter2,(void*)&(buf[i+3]),2); 
							memcpy((void*)&letter3,(void*)&(buf[i+5]),2); 
						memcpy((void*)&letter4,(void*)&(buf[i+7]),2); 
							break;
			 default:break;
        }

				step = 0;
			
				break;
			 	
		}
		
	}
     
}
