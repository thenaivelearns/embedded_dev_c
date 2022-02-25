#include "sx1278.h"
/**
  ******************************************************************************
  * @file    sx1278.c
  * @author  LiuYang
  * @brief   Lora模块
  *
  *
  ******************************************************************************
  * @attention
  *	Lora模块 串口3 默认波特率: 9600
	* E32-170T30D 170MHz 0x0000 0x28 2.4kbps 9600 8N1 1W
  *	模块有四种工作模式，由引脚 M0、 M1 设置；详细情况如下表所示：
	*	模式（0-3）       M0      M1                  模式介绍                                               备注
	*	0 一般模式        0       0            串口打开，无线打开，透明传输                           接收方必须是模式 0、 1
	*
	*	1 唤醒模式        1       0						 串口打开，无线打开；                                  接收方可以是模式 0
	*	                                       和模式 0 唯一区别：数据包发射前，自动增加唤醒码，       接收方可以是模式 1
	*                                        这样才能唤醒工作在模式 2 的接收方
  *
	*	2 省电模式        0       1            串口接收关闭，无线处于空中唤醒模式，收到无线数          发射方必须模式 1
	* 																			 据后，打开串口发出数据。                               该模式下不能发射
	* 
	* 
	* 3 休眠模式        1        1           模块进入休眠，可以接收参数设置命令                     
	详见工作参数详解
  *
  ******************************************************************************
	
	
  */


char stringstore[30]={0};
SX1278_Parse_State_t sx1278_parse; 
SX1278_Info_t        sx1278_info;
UsartRec_t SX1278_Rec;
void SX1278_GetWorkingParam();

void SX1278_Init(void)
{
 HAL_UART_Receive_DMA(&SX1278_USART, SX1278_Rec.RX_pData, sizeof(SX1278_Rec.RX_pData));  
	__HAL_UART_ENABLE_IT(&SX1278_USART, UART_IT_IDLE); 
		HAL_Delay(50);	
		SX1278_GetWorkingParam();
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);
}
static void unpackData(uint8_t *buf,u16 len)
{
	switch(sx1278_parse)
	{
		case SX1278_GET_WORK_STA:
			if(buf[0]==0xC0){
				sx1278_info.address = buf[1]<<8|buf[2];
				sx1278_info.baud = buf[1]<<8|buf[2];
			}
			break;
		case SX1278_GET_VERSION:break;
		case SX1278_RESET:break;
		default:break;
	}
}


static uint32_t temp;
void HAL_UART3_Receive_IDLE(UART_HandleTypeDef *huart)
{
//	
//  HAL_UART_Receive_IT(huart,aRxBuffer,1);          // Enable the USART5 Interrupt
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  
        SX1278_Rec.RX_Size =  RX_LEN - temp;   
        SX1278_Rec.RX_flag=1;  
 			  SX1278_unpackData(SX1278_Rec.RX_pData,SX1278_Rec.RX_Size);
		   //	memset(SX1278_Rec.RX_pData,'\0',RX_LEN);
        HAL_UART_Receive_DMA(huart,SX1278_Rec.RX_pData,sizeof(SX1278_Rec.RX_pData));  
    }
} 

void SX1278_GetWorkingParam(void)
{
	u8 cmd[] = {0xC0,0x00,0x17,0x28,0x1b,0x44};		//17不行就00
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_SET);
	HAL_Delay(50);	
	HAL_UART_Transmit(&SX1278_USART, cmd, 6, 0x100);
	
	sx1278_parse = SX1278_GET_WORK_STA;
}


void SX1278_GetData()
{

	 
}
void SX1278_SendData(uint8_t buf[],u16 len)
{
	HAL_UART_Transmit(&SX1278_USART,buf, len ,0x100);
}

void SX1278_Test_Demo(void)
{
	
	SX1278_Init();
	HAL_Delay(50);		
	SX1278_GetWorkingParam();
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);
		HAL_Delay(50);	

while(1)
{		int a=1;
	sprintf(stringstore,"我是%d个帅哥\r\n",a);
		SX1278_SendData((uint8_t*)stringstore,strlen(stringstore));
		HAL_Delay(50);		
}
}