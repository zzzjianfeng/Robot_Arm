#include "driver.h"
#include "Usart.h"
#include "Conveyor.h"

extern uint8_t temp_Arm;

extern uint8_t eye;

static uint8_t xw1=0,xw2=0,xw3=0,xw4=0;
uint32_t acc[4]={15000,15000,15000,15000};
int32_t spd[4]={17000,17000,17000,17000};
uint32_t dec[4]={5000,5000,5000,5000};

uint32_t Init_acc[4]={6000,6000,6000,6000};
int32_t Init_spd[4]={8000,8000,8000,8000};
uint32_t Init_dec[4]={6000,6000,6000,6000};


int32_t Init[4]={35800,-16000,-16000,16000};
int32_t Init2[4]={1,1,1,1};

int32_t MOVE1[4]={-44800,200,-1600,-2600};
int32_t MOVE2[4]={-1,3600,-1300,21700};

int32_t MOVE1_1[4]={-44800,1,-1,-1};
int32_t MOVE1_2[4]={1,200,-1,-1};
int32_t MOVE1_3[4]={-1,1,-1600,-1};
int32_t MOVE1_4[4]={-1,1,-1,-2600};
int32_t MOVE4[4]={26800,1,1,1};
int32_t MOVE3[4]={-49800,1,1,1};
int32_t MOVE5[4]={32000,1,1,1};
int32_t MOVE6[4]={1,1,3600,1};
int32_t MOVE7[4]={1,400,400,-400};
int32_t MOVE7_1[4]={1,800,800,-800};
int32_t MOVE8[4]={1,-800,-800,800};
int32_t MOVE8_1[4]={1,-400,-400,400};
int32_t MOVE9[4]={1,1,-1,-21700};

int32_t MOVE_A_1[4]={-44800,1,1,1};

int32_t MOVE_A_5[4]={1,2600,-800,-11400};

int32_t MOVE_A_TP5[4]={1,1800,-800,-7800};

int32_t MOVE_A_2[4]={1,2600,1,1};
int32_t MOVE_A_3[4]={1,1,-800,1};
int32_t MOVE_A_4[4]={1,1,1,-11400};

int32_t MOVE_A[4]={-44800,2800,-800,-11400};
int32_t MOVE_BCEF[4]={-1,2400,1500,26500};
int32_t MOVE_B[4]={-1,1,2800,-1};
int32_t MOVE_C[4]={-1,1,-1,4800};
int32_t MOVE_D[4]={-1,1,1200,1};
int32_t MOVE_E[4]={-1,-1400,1,1};
int32_t MOVE_F[4]={-1,-1,200,1};
int32_t MOVE_TP[4]={-1,-800,1,3600};	//4;3600 2:-800

typedef struct {
  uint16_t  Pulse_Pin ; 	
  uint32_t  Channel;		 
  uint32_t  IT_CCx ;  		
  uint32_t  Flag_CCx ;  
}Tim;

extern u8 state[4],mode;

int32_t  position[4]   ={0,0,0,0};           

speedRampData srd[4] ={
 {STOP,0,0,0,0,0,0},   
 {STOP,0,0,0,0,0,0},
 {STOP,0,0,0,0,0,0},
 {STOP,0,0,0,0,0,0}
};

const Tim Timer[4]={\
{STEPMOTOR_TIM_PULSE_PIN_X, STEPMOTOR_TIM_CHANNEL1, STEPMOTOR_TIM_IT_CC1, STEPMOTOR_TIM_FLAG_CC1},
{STEPMOTOR_TIM_PULSE_PIN_Y, STEPMOTOR_TIM_CHANNEL2, STEPMOTOR_TIM_IT_CC2, STEPMOTOR_TIM_FLAG_CC2},
{STEPMOTOR_TIM_PULSE_PIN_Z, STEPMOTOR_TIM_CHANNEL3, STEPMOTOR_TIM_IT_CC3, STEPMOTOR_TIM_FLAG_CC3},
{STEPMOTOR_TIM_PULSE_PIN_R, STEPMOTOR_TIM_CHANNEL4, STEPMOTOR_TIM_IT_CC4, STEPMOTOR_TIM_FLAG_CC4},
};

/********** BJ1 �˿ڶ��� **************
//DRIVER_DIR   PD11 
//DRIVER_OE    PD10
//STEP_PULSE   PE9 (TIM1_CH1)
******************************************/
static void STEPMOTOR1_Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_EN_PIN1|STEPMOTOR_DRIVER_DIR_PIN1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);									
	GPIO_ResetBits(GPIOC,STEPMOTOR_DRIVER_EN_PIN1);						
	
	RCC_APB2PeriphClockCmd(STEP_PULSE_RCC, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_TIM_PULSE_PIN_X;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(STEP_PULSE_PORT, &GPIO_InitStructure);					
}

/********** BJ2 �˿ڶ��� **************
//DRIVER_DIR   PD12 
//DRIVER_OE    PE10
//STEP_PULSE   PE11 (TIM1_CH2)
******************************************/
static void STEPMOTOR2_Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(STEP_PULSE_RCC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_DIR_PIN2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);										
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_TIM_PULSE_PIN_Y;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(STEP_PULSE_PORT, &GPIO_InitStructure);					

	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_EN_PIN2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,STEPMOTOR_DRIVER_EN_PIN2);						


}

/********** BJ3 �˿ڶ��� **************
//DRIVER_DIR   PD14
//DRIVER_OE    PE12
//STEP_PULSE   PE13 (TIM1_CH3)
******************************************/
static void STEPMOTOR3_Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(STEP_PULSE_RCC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_DIR_PIN3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);										
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_TIM_PULSE_PIN_Z;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(STEP_PULSE_PORT, &GPIO_InitStructure);						

	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_EN_PIN3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,STEPMOTOR_DRIVER_EN_PIN3);						


}
	
/********** BJ4 �˿ڶ��� **************
//DRIVER_DIR   PC13
//DRIVER_OE    PC12
//STEP_PULSE   PE14 (TIM1_CH4)
******************************************/
static void STEPMOTOR4_Driver_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(STEP_PULSE_RCC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_DIR_PIN4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);										
	
	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_TIM_PULSE_PIN_R;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(STEP_PULSE_PORT, &GPIO_InitStructure);					

	GPIO_InitStructure.GPIO_Pin = STEPMOTOR_DRIVER_EN_PIN4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,STEPMOTOR_DRIVER_EN_PIN4);						


}

void EXTI_INIT(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
    GPIO_Init(GPIOE,&GPIO_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line=EXTI_Line4;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);
}

void EXTI4_IRQHandler(void)
{
	// static uint16_t i=0;
    if(EXTI_GetITStatus(EXTI_Line4)==SET)
    {
		// i++;
		// if (i==1)
		// {
		// 	if ((eye==1)&&(strcmp(RxPacket,"1")==0||strcmp(RxPacket,"2")==0||strcmp(RxPacket,"3")==0))
		// 	{
		// 		Conveyor_OFF();
		// 		eye=0;
		// 		// temp_Arm=1;
		// 	}
		// }
		// else 
		// {
			// if ((RXD_ReceiveFlag==1)&&(strcmp(RxPacket,"1")==0||strcmp(RxPacket,"2")==0||strcmp(RxPacket,"3")==0))
			// {
			// 	Conveyor_OFF();
			// 	// temp_Arm=1;
			// }
		// }
		if ((eye==1)&&(strcmp(RxPacket,"1")==0||strcmp(RxPacket,"2")==0||strcmp(RxPacket,"3")==0))
		{
			Conveyor_OFF();
			eye=0;
			temp_Arm=1;
		}
		else {temp_Arm=0;eye=0;}
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void limit_switch_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 		
	GPIO_Init(GPIOE, &GPIO_InitStructure);									
}


static void STEPMOTOR_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void TIM1_PWM_Init(void)
{	
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	
	STEPMOTOR1_Driver_Init();
	STEPMOTOR2_Driver_Init();
	STEPMOTOR3_Driver_Init();
	STEPMOTOR4_Driver_Init();
	
	STEPMOTOR_NVIC_Init();
	
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure.TIM_Prescaler =36-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable);
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Disable);
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Disable);

	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Disable);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE); 
	TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE); 
	TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC4, DISABLE);

	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	TIM_Cmd(TIM1,ENABLE);   
 }

 
void timdisable(int8_t Axis)
{
	
	switch(Axis)
	{
		case 0:TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
		       TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable);TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);break;
		case 1:TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);TIM_ClearFlag(TIM1,TIM_FLAG_CC2);
		       TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Disable);TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE);break;
		case 2:TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);TIM_ClearFlag(TIM1,TIM_FLAG_CC3);
		       TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Disable);TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);break;
		case 3:TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);TIM_ClearFlag(TIM1,TIM_FLAG_CC4);
		       TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Disable);TIM_ITConfig(TIM1, TIM_IT_CC4, DISABLE);break;
		default:break;
	}
}

void timopen(int8_t Axis)
{
	switch(Axis)
	{
		case 0:TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
			TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);break;
		case 1:TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);TIM_ClearFlag(TIM1,TIM_FLAG_CC2);
		  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);break;
		case 2:TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);TIM_ClearFlag(TIM1,TIM_FLAG_CC3);
			TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);break;
		case 3:TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);TIM_ClearFlag(TIM1,TIM_FLAG_CC4);
			TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);break;
		default:break;
	}
}

void AxisMove(int32_t Axis,int32_t step, uint32_t accel, uint32_t decel, int32_t speed)
{
	
  uint32_t max_s_lim=0;

  uint32_t accel_lim=0;
	float ftemp=0.0;
	timopen(Axis);
	
	if(step == 0)
	{
			return ;
	}
	
  else	if(step <0) 
  {
    srd[Axis].dir = CCW;     
    STEPMOTOR_DRIVER_DIR1=srd[0].dir; 
	STEPMOTOR_DRIVER_DIR2=srd[1].dir;      
	STEPMOTOR_DRIVER_DIR3=srd[2].dir;
	STEPMOTOR_DRIVER_DIR4=srd[3].dir;
    step =-step;       
  }
  else 
  {
    srd[Axis].dir = CW;     
	  STEPMOTOR_DRIVER_DIR1=srd[0].dir; 
	  STEPMOTOR_DRIVER_DIR2=srd[1].dir;      
	  STEPMOTOR_DRIVER_DIR3=srd[2].dir;
	  STEPMOTOR_DRIVER_DIR4=srd[3].dir;
  }
	
	if(step == 1) 
  {
    srd[Axis].accel_count = -1;
    
    srd[Axis].run_state = DECEL;
    
    srd[Axis].step_delay = 1000;
		
  }
  if(step != 0)
  {
		
    srd[Axis].min_delay = T1_FREQ/speed/2;
    srd[Axis].step_delay = (int32_t)(((long)T1_FREQ*0.676* sqrt(2000000 / accel))/1000/2);
    max_s_lim = speed*speed/(2*accel);
    if(max_s_lim == 0){
      max_s_lim = 1;
    }
    if((accel+decel)>step)
		{
			ftemp=(float)decel/(float)(accel+decel);
			accel_lim = (float)step*ftemp;
		}
		else
		{
			accel_lim = step/(accel+decel)*decel;
		}
		
    if(accel_lim == 0){
      accel_lim = 1;
    }

    if(accel_lim <= max_s_lim){
      srd[Axis].decel_val = accel_lim - step;
    }
    else{
      srd[Axis].decel_val =-(int32_t)(max_s_lim*accel/decel);
    }
    
    if(srd[Axis].decel_val == 0){
      srd[Axis].decel_val = -1;
    }
    srd[Axis].decel_start = step + srd[Axis].decel_val;
    if(srd[Axis].step_delay <= srd[Axis].min_delay)
	  {
      srd[Axis].step_delay = srd[Axis].min_delay;
      srd[Axis].run_state = RUN;
    }
    else{
      srd[Axis].run_state = ACCEL;
    }
    srd[Axis].accel_count = 0;
	}
	if(Axis==0)
	{
		TIM_SetCompare1(TIM1,100);
	}
	else if(Axis==1)
	{
		TIM_SetCompare2(TIM1,100);
	}
	else if(Axis==2)
	{
		TIM_SetCompare3(TIM1,100);
	}
	else if(Axis==3)
	{
		TIM_SetCompare4(TIM1,100);
	}
	TIM_SetAutoreload(TIM1,100);
	TIM_Cmd(TIM1, ENABLE);
}

void limitswitch(void)
{
		if((stop_XL)==0)
		{
			srd[0].run_state = STOP;
			position[0]=0;
			xw1=1;
		}
		if((stop_YU)==0)
		{
			srd[1].run_state = STOP;
			position[1]=0;
			xw2=1;
		}
		if((stop_ZF)==0)
		{ 
			srd[2].run_state = STOP;
			position[2]=0;
			xw3=1;
		}
		if ((stop_F)==0)
		{
			srd[3].run_state = STOP;
			position[3]=0;
			xw4=1;
		}
}

void Step_Motor_Contrl(uint32_t *accel,uint32_t *decel,int32_t *speed,int32_t *pulse_num)
{
	AxisMove(0,pulse_num[0],accel[0],decel[0],speed[0]);
	AxisMove(1,pulse_num[1],accel[1],decel[1],speed[1]);
	AxisMove(2,pulse_num[2],accel[2],decel[2],speed[2]);
	AxisMove(3,pulse_num[3],accel[3],decel[3],speed[3]);
    while(1)
	{
		if((srd[0].run_state == STOP)&&(srd[1].run_state == STOP)&&(srd[2].run_state == STOP)&&(srd[3].run_state == STOP))
		{return;}				           
	}
}

void Step_Motor_Init(uint32_t *accel,uint32_t *decel,int32_t *speed,int32_t *pulse_num)
{
	AxisMove(0,pulse_num[0],accel[0],decel[0],speed[0]);
	AxisMove(1,pulse_num[1],accel[1],decel[1],speed[1]);
	AxisMove(2,pulse_num[2],accel[2],decel[2],speed[2]);
	AxisMove(3,pulse_num[3],accel[3],decel[3],speed[3]);
    while(1)
	{
		limitswitch();
		if(((xw1==1)&&(xw2==1)&&(xw3==1)&&(xw4==1))||((srd[0].run_state == STOP)&&(srd[1].run_state == STOP)&&(srd[2].run_state == STOP)&&(srd[3].run_state == STOP)))
		{return;}
	}
}

void MOVE_1_(void)
{
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE1_2);
	Step_Motor_Contrl(acc,dec,spd,MOVE1_3);
	Step_Motor_Contrl(acc,dec,spd,MOVE1_4);
	Step_Motor_Contrl(acc,dec,spd,MOVE1_1);
	delay_ms(20);
	Servo_Run(2);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE4);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE2);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE3);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE7);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE8);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE7_1);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE8_1);
	delay_ms(20);
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE5);

	Step_Motor_Contrl(acc,dec,spd,MOVE6);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE9);
}


void MOVE_2_(void)
{
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE_A_TP5);
	Step_Motor_Contrl(acc,dec,spd,MOVE_A_1);
	delay_ms(20);
	Servo_Run(1);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE4);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE_BCEF);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE3);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE7);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE8);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE7_1);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE8_1);
	delay_ms(20);
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl(acc,dec,spd,MOVE5);

	Step_Motor_Contrl(acc,dec,spd,MOVE_D);
	 
	Step_Motor_Contrl(acc,dec,spd,MOVE9);
	 
}

void MOVE_INIT(void)
{
	xw1=0;xw2=0;xw3=0;xw4=0;
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl(Init_acc,Init_dec,Init_spd,Init2);
	delay_ms(20);
	Step_Motor_Init(Init_acc,Init_dec,Init_spd,Init);
	delay_ms(20);
	Step_Motor_Contrl(Init_acc,Init_dec,Init_spd,Init2);
	delay_ms(20);
}

void TIM1_CC_IRQHandler(void)
{
	uint8_t Axis=0;
	
  uint16_t new_step_delay[4]={0,0,0,0};
	
  static uint16_t last_accel_delay[4]={0,0,0,0};
	
  static uint32_t step_count[4] ={0,0,0,0};
	
  static int32_t rest[4] ={0,0,0,0};
  static uint8_t i[4]={0,0,0,0};
	
	

  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
		Axis=0;
		
		TIM_SetCompare1(TIM1,srd[Axis].step_delay);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
  }
	else if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
  {
		Axis=1;
		TIM_SetCompare2(TIM1,srd[Axis].step_delay);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    TIM_ClearFlag(TIM1,TIM_FLAG_CC2);
  }
	else if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
  {
		Axis=2;
		TIM_SetCompare3(TIM1,srd[Axis].step_delay);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    TIM_ClearFlag(TIM1,TIM_FLAG_CC3);
  }
	else if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
  {
		Axis=3;
		TIM_SetCompare4(TIM1,srd[Axis].step_delay);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    TIM_ClearFlag(TIM1,TIM_FLAG_CC4);
  }	
	
  TIM_SetAutoreload(TIM1,srd[Axis].step_delay);
	
  if(srd[Axis].run_state)
  {
		switch(Axis)
		{
			case 0:
				if(GPIO_ReadOutputDataBit(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_X))
					{
						GPIO_ResetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_X);
					}
					else
					{
						GPIO_SetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_X);
					}break;
			case 1:
				if(GPIO_ReadOutputDataBit(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Y))
					{
						GPIO_ResetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Y);
					}
					else
					{
						GPIO_SetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Y);
					}break;
			case 2:
				if(GPIO_ReadOutputDataBit(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Z))
					{
						GPIO_ResetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Z);
					}
					else
					{
						GPIO_SetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_Z);
					}break;
			case 3:
				if(GPIO_ReadOutputDataBit(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_R))
					{
						GPIO_ResetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_R);
					}
					else
					{
						GPIO_SetBits(STEP_PULSE_PORT, STEPMOTOR_TIM_PULSE_PIN_R);
					}break;					
			default:break;
		}
	}
  i[Axis]++;  
  if(i[Axis]==2) 
  {
	  i[Axis]=0; 
    switch(srd[Axis].run_state) 
    {
			
      case STOP:   
			  step_count[Axis] = 0; 
        rest[Axis] = 0; 
			  
        timdisable(Axis);
        last_accel_delay[Axis] = 0;
        srd[Axis].accel_count = 0;
        srd[Axis].step_delay = 0;
        srd[Axis].min_delay = 0;
				
        break;
      
      case ACCEL:
				TIM_CCxCmd(TIM1,Timer[Axis].Channel,TIM_CCx_Enable);
        step_count[Axis]++;
			  srd[Axis].accel_count++;
			
        if(srd[Axis].dir==CW)
        {	  	
          position[Axis]++;
        }
        else
        {
          position[Axis]--;
        }
				
        new_step_delay[Axis] = srd[Axis].step_delay - (((2 * (long)srd[Axis].step_delay) + rest[Axis])/(4 * srd[Axis].accel_count + 1));
        rest[Axis] = ((2 * (long)srd[Axis].step_delay)+rest[Axis])%(4 * srd[Axis].accel_count + 1);	
				
				
        if(step_count[Axis] >= srd[Axis].decel_start)
        {
          srd[Axis].accel_count = srd[Axis].decel_val;
          srd[Axis].run_state = DECEL;
        }
				
				if(new_step_delay[Axis] <= srd[Axis].min_delay)  
        {
          last_accel_delay[Axis] = new_step_delay[Axis];
          new_step_delay[Axis] = srd[Axis].min_delay;
          rest[Axis] = 0;
          srd[Axis].run_state = RUN;
        }
        break;
      
      case RUN:
				step_count[Axis]++;
        if(srd[Axis].dir==CW)
        {	  	
          position[Axis]++;
        }
        else
        {
          position[Axis]--;
        }
        new_step_delay[Axis] = srd[Axis].min_delay;
				
        if(step_count[Axis] >= srd[Axis].decel_start)
        {
					step_count[Axis]= srd[Axis].decel_start;
          srd[Axis].accel_count = srd[Axis].decel_val;
         
          new_step_delay[Axis] = last_accel_delay[Axis];
          srd[Axis].run_state = DECEL;
        }
        break;
			
      case DECEL:
        step_count[Axis]++;
        if(srd[Axis].dir==CW)
        {	  	
          position[Axis]++;
        }
        else
        {
          position[Axis]--;
        }
        srd[Axis].accel_count++;
        new_step_delay[Axis] = srd[Axis].step_delay - (((2 * (long)srd[Axis].step_delay) + rest[Axis])/(4 * srd[Axis].accel_count + 1));
        rest[Axis] = ((2 * (long)srd[Axis].step_delay)+rest[Axis])%(4 * srd[Axis].accel_count + 1);

				
        if(srd[Axis].accel_count >= 0)
        {
          srd[Axis].run_state = STOP;
        }
        break;

    }
    srd[Axis].step_delay = new_step_delay[Axis];
  }
}
