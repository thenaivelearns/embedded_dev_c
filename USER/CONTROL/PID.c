#include "PID.h"

PID_t Deep_pid;
PID_t Heading_pid;
PID_t Pitch_pid;
PID_t findline_pid;

                              //kp,ki,kd,     p_max,      p_min,     i_max,     i_min,    d_max,     d_min,  out_max,  out_min   ,ËÀÇø
float Heading_pid_param[12]={ -1 ,    0,   -3,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float    Deep_pid_param[12]={ -15, -3,   0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float   Pitch_pid_param[12]={ - 30 , 0, -30,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float   findlineparam[12]={  -1 , 0, -3,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
void PID_Init(void)                                                //PID  - ³õÊ¼»¯
{
	
  PID_Clear(&Deep_pid);
	PID_Clear(&Pitch_pid);
	PID_Clear(&Heading_pid);
	PID_Param_Init(&Heading_pid,Heading_pid_param);   
	PID_Param_Init(&Pitch_pid,Pitch_pid_param);
	PID_Param_Init(&Deep_pid,Deep_pid_param);   
	PID_Param_Init(&findline_pid,findlineparam);  
}
 
void PID_Param_Init(PID_t *pid, float *pid_param) 
{
	pid->Kp = pid_param[0];
	pid->Ki = pid_param[1];;
	pid->Kd = pid_param[2];;
	pid->p_OutMAX = pid_param[3];
	pid->p_OutMIN = pid_param[4];
	pid->i_OutMAX = pid_param[5];
	pid->i_OutMIN = pid_param[6];
	pid->d_OutMAX = pid_param[7];
	pid->d_OutMIN = pid_param[8];
	pid->OutPutMAX = pid_param[9];
	pid->OutPutMIN = pid_param[10];
	pid->dead_zone = pid_param[11];
}
void PID_Clear(PID_t *pid)                 
{
  pid->Bias = 0;
	pid->Last_Bias = 0;
	pid->Last_Last_Bias = 0;
	pid->p_Out = 0;
	pid->i_Out = 0;
	pid->d_Out = 0;
	pid->OutPut = 0;
} 
float Position_PID_Cal(float tar,float input,PID_t *pid)
{
	pid->Bias = tar -input;
	
	pid->p_Out = pid->Kp*pid->Bias;
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN); 
	
	pid->i_Out+= pid->Ki*pid->Bias;                                                          
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - pid->Last_Bias);
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
	
	pid->OutPut  = pid->p_Out+pid->i_Out+pid->d_Out;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);
	
	pid->Last_Bias = pid->Bias;						
  return pid->OutPut;
}	
float Incremental_PID_Cal(float tar,float input,PID_t *pid)        
{
	float increment = 0;
	pid->Bias = tar - input ;

	pid->p_Out = pid->Kp*(pid->Bias-pid->Last_Bias);                                      
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN); 
	
	pid->i_Out = pid->Ki*pid->Bias;                                                          
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - 2*pid->Last_Bias + pid->Last_Last_Bias);
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
		
	increment = pid->p_Out+pid->i_Out+pid->d_Out; 
	
	if(fabs(pid->Bias)< pid->dead_zone){
		
		increment = 0;
	}
	pid->OutPut += increment;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);
	
	pid->Last_Last_Bias = pid->Last_Bias;
	pid->Last_Bias = pid->Bias;						
  return pid->OutPut;
}
 
