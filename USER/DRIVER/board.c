#include "board.h"
QueueHandle_t IMU_Message_Queue = NULL;
QueueHandle_t Control_Message_Queue = NULL;  
QueueHandle_t Command_Message_Queue = NULL;  
QueueHandle_t Pressure_Message_Queue = NULL; 
QueueHandle_t TempHum_Message_Queue = NULL;
QueueHandle_t Battery_Message_Queue = NULL; 
/**
  * @brief  
  * @param  
  * @note  
  */
void delay_ms(int nms)   
{
	if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
	{
		HAL_Delay(nms);
	}else
	{
		osDelay(nms);
	}
}
/**
  * @brief  任务消息队列、信号量、定时器创建 
  * @param  
  * @note   1. 在各个任务执行前创建 2. 注意防止中断打断创建
  */	
void Task_Queue_Semaphore_Timers_Create(void)
{
	do{Pressure_Message_Queue = xQueueCreate(1, sizeof(PressureMsg_t));
	}while(Pressure_Message_Queue==NULL);
	
	do{
		IMU_Message_Queue = xQueueCreate(1, sizeof(IMUMsg_t));
	}while(IMU_Message_Queue==NULL);
	
	do{
		Control_Message_Queue = xQueueCreate(3, sizeof(ControlMsg_t));
	}while(Control_Message_Queue==NULL);
	
	do{
		TempHum_Message_Queue = xQueueCreate(1, sizeof(TempHumiMsg_t));
	}while(TempHum_Message_Queue==NULL);
	
	do{
			Battery_Message_Queue = xQueueCreate(1, sizeof(PowerMsg_t));	
	}while(Battery_Message_Queue==NULL);
	
}
/**
  * @brief  
  * @param  
  * @note  
  */	
 
void BSP_Init(void)
{	

	PWM_Init();
	Batt_Init();
	PWM_Init();
	SX1278_Init();
	JY901_Init();
	HP206C_Init();
	SHT35_Init();
	PID_Init();
  Stm32_Pi_Init();
 
	//测试使用
//	HP206C_Test_Demo();  //压传测试demo
//	SW_Test_Demo();      //自锁开关demo
	
//	LED_Test_Demo();     //led 灯demo
	//PWM_Test_Demo();     // Tim1 ch2 有干扰
 //SX1278_Test_Demo();  //Lora 模块  
//	Batt_Test_Demo();    //电量demo
//  SHT35_TestDemo();    //SHT35温湿度demo  
//	JY901_Test_Demo();   //电子罗盘demo
//	PI_STM32_Test_Demo();//树莓派串口demo





}
/**
  * @brief  
  * @param  
  * @note  
  */
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)//JY901
	{
		HAL_UART1_Receive_IDLE(huart);
	}
	if(huart->Instance == USART2)//树莓派
	{
		HAL_PI_To_UART_Receive_IDLE(huart);
	}
	if(huart->Instance == USART3)//Lora模块 SX1278
	{
	 HAL_UART3_Receive_IDLE(huart);
	}
	if(huart->Instance == UART4)//
	{
		
		
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static u8 sw = 0;
	if(HAL_GPIO_ReadPin(POWER_KEY_STATE_GPIO_Port,POWER_KEY_STATE_Pin) == GPIO_PIN_RESET)
	{
		if(sw == 0)
		{
			POWER_KEY(1);//开机 - 锁存
			sw = 1;
		}else{
			 POWER_KEY(0);//关机	
			sw = 0;			
		}
		
	}
	
	
}
	
 