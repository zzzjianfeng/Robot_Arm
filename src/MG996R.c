#include "MG996R.h"

void MG996R1_Init(uint16_t arr,uint16_t psc) //arr=20000-1,psc=72-1
{
	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(MG996R1_Gpio_Clk,ENABLE);
    RCC_APB1PeriphClockCmd(MG996R1_Tim_Clk,ENABLE);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin=MG996R1_Pin;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MG996R1_Port,&GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=arr-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(MG996R1_Tim,&TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse=0;
    TIM_OC1Init(MG996R1_Tim,&TIM_OCInitStruct);

    TIM_Cmd(MG996R1_Tim,ENABLE);
}

void MG996R2_Init(uint16_t arr,uint16_t psc) //arr=20000-1,psc=72-1
{
	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(MG996R2_Gpio_Clk,ENABLE);
    RCC_APB1PeriphClockCmd(MG996R2_Tim_Clk,ENABLE);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin=MG996R2_Pin;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MG996R2_Port,&GPIO_InitStruct);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=arr-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(MG996R2_Tim,&TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse=0;
    TIM_OC2Init(MG996R2_Tim,&TIM_OCInitStruct);

    TIM_Cmd(MG996R2_Tim,ENABLE);
}

void Servo_Init(void)
{
	MG996R1_Init(20000, 72);
    MG996R2_Init(20000, 72);
}

void Servo_SetLength(u8 Servox, float Length)
{
	switch(Servox)
	{
		case 0: TIM_SetCompare1(TIM4, Length);break;
		case 1: TIM_SetCompare2(TIM4, Length);break;
	}
}

void Servo_Run(u8 Mode)
{
	switch(Mode)
	{
		case 0: 	Servo_SetLength(0, 500);	Servo_SetLength(1, 1500);break;		//复位
		case 1: 	Servo_SetLength(0, 2000);	Servo_SetLength(1, 1500);break;		//
		case 2: 	Servo_SetLength(0, 500);	Servo_SetLength(1, 450);break;		//抓三角
		case 3: 	Servo_SetLength(0, 300);	Servo_SetLength(1, 150);break;		//抓方
	}
}
