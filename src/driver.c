#include "driver.h"

extern uint8_t temp_Arm;

extern uint8_t eye;

extern uint8_t eye2;

static uint8_t xw1=0,xw2=0,xw3=0,xw4=0;
uint32_t acc=10000;
int32_t spd=30000;
uint32_t dec=10000;

uint32_t acc2=6000;
int32_t spd2=10000;
uint32_t dec2=6000;

uint32_t Init_acc=3000;
int32_t Init_spd=6000;
uint32_t Init_dec=3000;


int32_t Init[4]={35800,-16000,-16000,16000};
int32_t Init3[4]={35800,16000,-16000,-16000};
int32_t Init2[4]={-400,1,1,1};

int32_t MOVE0_3[4]={-49800,1,1,21000};

int32_t MOVE0_3_2[4]={-49800,1,1,26800};

// int32_t MOVE_zfx[4]={-44800,200,-1600,-2600};
// int32_t MOVE2[4]={-49800,2000,-1600,21700};
/*正方形收尾动作*/
// int32_t MOVEsw[4]={-0,0,3600,-21700};
/*三角和圆收尾动作*/
// int32_t MOVEsw2[4]={-1,1,1200,-21700};

// int32_t MOVE_sj[4]={-44800,2800,-800,-7800};
// int32_t MOVE_yuan[4]={-44800,2200,-800,-7800};
// int32_t MOVE_B[4]={-49800,600,1200,26500};

int32_t MOVE2_2=1600;
int32_t MOVE2_3=-1700;
int32_t MOVE2_4=21700;

int32_t MOVE1_1=-44800;
int32_t MOVE1_2=1050;
int32_t MOVE1_3=-1400;
int32_t MOVE1_4=-2300;
int32_t MOVE4=26800;
int32_t MOVE3=-49800;
int32_t MOVE5=58000;
int32_t MOVE6_3=3600;
/*抖动*/
int32_t MOVE7[4]={1,1,200,-1600};
int32_t MOVE7_1[4]={1,1,400,-1400};
int32_t MOVE8[4]={1,-1,-400,1400};
int32_t MOVE8_1[4]={1,-1,-200,1600};

int32_t MOVE7a[4]={1,300,200,-1600};
int32_t MOVE7_1a[4]={1,600,400,-1400};
int32_t MOVE8a[4]={1,-600,-400,1400};
int32_t MOVE8_1a[4]={1,-300,-200,1600};

int32_t MOVE9_4=-15000;
int32_t MOVE9_5=-3000;

int32_t MOVE_A_1=-44800;
int32_t MOVE_A_2=2700;
int32_t MOVE_A_2_2=2800;
int32_t MOVE_A_3=-1100;
int32_t MOVE_A_3_2=-1000;
int32_t MOVE_A_4=-8300;
int32_t MOVE_A_4_2=-8200;

int32_t MOVE_B_2=100;
int32_t MOVE_B_2_2=-200;
int32_t MOVE_B_3=1300;
int32_t MOVE_B_3_2=1200;
int32_t MOVE_B_4=27000;
int32_t MOVE_D_3=1200;

typedef struct {
  uint16_t  Pulse_Pin ; 	
  uint32_t  Channel;		 
  uint32_t  IT_CCx ;  		
  uint32_t  Flag_CCx ;  
}Tim;

int32_t  position[4]  ={0,0,0,0};           

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
/*光电开关初始化*/
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

void EXTI_INIT2(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
    GPIO_Init(GPIOE,&GPIO_InitStruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line=EXTI_Line5;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);
}
/*传送带上的光电传感器判断视觉是否正确输入*/
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4)==SET)
    {
		if ((eye==1)&&(strcmp(RxPacket,"1")==0||strcmp(RxPacket,"2")==0||strcmp(RxPacket,"3")==0))
		{
			// delay_ms(10);
			Conveyor_OFF();
			eye=0;
			temp_Arm=1;
		}
		else {temp_Arm=0;eye=0;}
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
/*机架上的光电传感器判断是否正确放入*/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5)==SET)
    {
		if (eye2==1)
		{
			Dial[Dial_Now]=0;
			eye2=0;
		}
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}
/*限位开关初始化*/
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*机械臂步进电机初始化*/
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
	TIM1->BDTR = TIM_BDTR_MOE;
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
  if(step == 0)
  {
    return ;
  }
  srd[Axis].run_state = ACCEL;
  timopen(Axis);
  if(step <0) 
  {
    srd[Axis].dir = CCW;     
    step =-step;       
  }
  else 
  {
    srd[Axis].dir = CW;     
  }
	switch(Axis)
	{
		case 0:
			STEPMOTOR_DRIVER_DIR1=srd[0].dir; // 控制电机方向
		break;
		case 1:
			STEPMOTOR_DRIVER_DIR2=srd[1].dir; // 控制电机方向
		break;
		case 2:
			STEPMOTOR_DRIVER_DIR3=srd[2].dir; // 控制电机方向
		break;
		case 3:
			STEPMOTOR_DRIVER_DIR4=srd[3].dir; // 控制电机方向
		break;		
	}	
  if(step == 1) 
  {
    srd[Axis].accel_count = -1;
    
    srd[Axis].run_state = DECEL;
    
    srd[Axis].step_delay = 1000;
		
  }
  else if(step != 0)
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

uint8_t limitsw_Num(uint8_t Num)
{
	if((stop(Num))==0)
	{
		srd[Num].run_state = STOP;
		position[Num]=0;
		return 1;
	}
	else return 0;
}

void Step_Motor_Contrl(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num)
{
	AxisMove(0,pulse_num[0],accel,decel,speed);
	AxisMove(1,pulse_num[1],accel,decel,speed);
	AxisMove(2,pulse_num[2],accel,decel,speed);
	AxisMove(3,pulse_num[3],accel,decel,speed);
    while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		if((srd[0].run_state == STOP)&&(srd[1].run_state == STOP)&&(srd[2].run_state == STOP)&&(srd[3].run_state == STOP))
		{return;}
	}
}

void Step_Motor_Contrl2(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num)
{
	AxisMove(1,pulse_num[1],accel,decel,speed);
	AxisMove(3,pulse_num[3],accel,decel,speed);
    while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		if((srd[1].run_state == STOP)&&(srd[3].run_state == STOP))
		{return;}
	}
}

void Step_Motor_Contrl1_4(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num)
{
	AxisMove(3,pulse_num[3],accel,decel,speed);
	AxisMove(0,pulse_num[0],accel,decel,speed);
    while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		if((srd[0].run_state == STOP)&&(srd[3].run_state == STOP))
		{return;}
	}
}

void Step_Motor_Contrl_Alone(uint8_t Num,uint32_t accel,uint32_t decel,int32_t speed,int32_t pulse_num)
{
	delay_ms(20);
	AxisMove(Num,pulse_num,accel,decel,speed);
	while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		if(srd[Num].run_state == STOP)
		{return;}
	}
}

void Step_Motor_Init_Alone(uint8_t Num,uint32_t accel,uint32_t decel,int32_t speed,int32_t pulse_num)
{
	AxisMove(Num,pulse_num,accel,decel,speed);
	while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		if((limitsw_Num(Num)==1)||(srd[Num].run_state == STOP))
		{return;}
	}
}

void Step_Motor_Init(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num)
{
	AxisMove(1,pulse_num[1],accel,decel,speed);
	AxisMove(2,pulse_num[2],accel,decel,speed);
	AxisMove(3,pulse_num[3],accel,decel,speed);
	AxisMove(0,pulse_num[0],accel,decel,speed);
    while(1)
	{
		// OLED_ShowSignedNum(1,8,position[0],5);
		// OLED_ShowSignedNum(2,1,position[1],5);
		// OLED_ShowSignedNum(3,1,position[2],5);
		// OLED_ShowSignedNum(4,1,position[3],5);
		limitswitch();
		if(((xw1==1)&&(xw2==1)&&(xw3==1)&&(xw4==1))||((srd[0].run_state == STOP)&&(srd[1].run_state == STOP)&&(srd[2].run_state == STOP)&&(srd[3].run_state == STOP)))
		{return;}
	}
}
/* 正方形抓取动作 */
void MOVE_1_(void)
{
	// delay_ms(100);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE1_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE1_3);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE1_4);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE1_1);
	delay_ms(20);
	Servo_Run(2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE4);
	delay_ms(20);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE2_2);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE2_3);
	// Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE2_4);
	// Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE3);
	Step_Motor_Contrl1_4(acc2,dec2,spd2,MOVE0_3);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8_1[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7_1[3]);
	delay_ms(20);
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE5);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE6_3);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE9_4);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE9_5);
	delay_ms(20);
}

/* 三角的抓取动作 */
void MOVE_2_(void)
{
	// Step_Motor_Contrl(acc,dec,spd,MOVE_sj);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE_A_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_A_3_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE_A_4);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE_A_1);
	delay_ms(20);
	Servo_Run(1);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE4);
	delay_ms(20);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE_B_2_2);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_B_3_2);
	// Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE_B_4);
	// Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE3);
	Step_Motor_Contrl1_4(acc2,dec2,spd2,MOVE0_3_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8_1a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7_1a[3]);
	delay_ms(20);
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE5);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_D_3);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE9_4);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE9_5);
	delay_ms(20);
}
/* 圆的抓取动作 */
void MOVE_3_(void)
{
	// Step_Motor_Contrl(acc,dec,spd,MOVE_sj);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE_A_2_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_A_3);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE_A_4_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE_A_1);
	delay_ms(20);
	Servo_Run(1);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE4);
	delay_ms(20);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE_B_2);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_B_3);
	// Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE_B_4);
	// Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE3);
	Step_Motor_Contrl1_4(acc2,dec2,spd2,MOVE0_3_2);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE8_1a[3]);
	delay_ms(20);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE7_1a[3]);
	delay_ms(20);
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Contrl_Alone(0,acc,dec,spd,MOVE5);
	Step_Motor_Contrl_Alone(2,acc,dec,spd,MOVE_D_3);
	Step_Motor_Contrl_Alone(3,acc,dec,spd,MOVE9_4);
	Step_Motor_Contrl_Alone(1,acc,dec,spd,MOVE9_5);
	delay_ms(20);
}

/* 复位动作 */
void MOVE_INIT(void)
{
	xw1=0;xw2=0;xw3=0;xw4=0;
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Init_Alone(0,Init_acc,Init_dec,Init_spd,Init2[0]);
	delay_ms(20);
	Step_Motor_Init(Init_acc,Init_dec,Init_spd,Init);
	delay_ms(20);
	Step_Motor_Init_Alone(3,Init_acc,Init_dec,Init_spd,Init2[3]);
	Step_Motor_Init_Alone(2,Init_acc,Init_dec,Init_spd,Init2[2]);
	Step_Motor_Init_Alone(1,Init_acc,Init_dec,Init_spd,Init2[1]);
}
/*开机动作*/
void MOVE_INIT3(void)
{
	xw1=0;xw2=0;xw3=0;xw4=0;
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Init_Alone(0,Init_acc,Init_dec,Init_spd,Init2[0]);
	delay_ms(20);
	Step_Motor_Init(Init_acc,Init_dec,Init_spd,Init3);
	delay_ms(20);
	Step_Motor_Init_Alone(3,Init_acc,Init_dec,Init_spd,Init2[3]);
	Step_Motor_Init_Alone(2,Init_acc,Init_dec,Init_spd,Init2[2]);
	Step_Motor_Init_Alone(1,Init_acc,Init_dec,Init_spd,Init2[1]);
}

void MOVE_INIT2(void)
{
	Servo_Run(0);
	delay_ms(20);
	Step_Motor_Init_Alone(3,Init_acc,Init_dec,Init_spd,Init[3]);
	Step_Motor_Init_Alone(2,Init_acc,Init_dec,Init_spd,Init[2]);
	Step_Motor_Init_Alone(1,Init_acc,Init_dec,Init_spd,Init[1]);
	Step_Motor_Init_Alone(0,Init_acc,Init_dec,Init_spd,Init[0]);
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
        last_accel_delay[Axis] = 0;
        srd[Axis].accel_count = 0;
        srd[Axis].step_delay = 0;
        srd[Axis].min_delay = 0;
		timdisable(Axis);
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
