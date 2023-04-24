#include "Dial.h"

uint32_t Point=0;
extern uint8_t temp_Arm;

void Dial_Init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    /*Dial CLOCK INIT*/
    RCC_APB2PeriphClockCmd(Dial_CLK,ENABLE);
    /*Dial EA GPIO INIT*/
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin=Dial_EN_PIN;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Dial_PORT,&GPIO_InitStruct);
    GPIO_ResetBits(Dial_PORT,Dial_EN_PIN);
    /*Dial DIR GPIO INIT*/
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin=Dial_DIR_PIN;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Dial_PORT,&GPIO_InitStruct);
    GPIO_ResetBits(Dial_PORT,Dial_DIR_PIN);
    /*Dial STEP GPIO INIT*/
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin=Dial_STEP_PIN;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Dial_PORT,&GPIO_InitStruct);
    /*Dial Tim Init*/
    RCC_APB2PeriphClockCmd(Dial_TIM_CLK,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=arr-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(Dial_TIM,&TIM_TimeBaseInitStruct);

    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse=0;
    TIM_OC3Init(Dial_TIM,&TIM_OCInitStruct);

    TIM_CtrlPWMOutputs(Dial_TIM,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=TIM8_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ITConfig(Dial_TIM,TIM_IT_Update,ENABLE);

    TIM_Cmd(Dial_TIM,ENABLE);
}

void Dial_TOGGLE(void)
{
    Dial_PORT->ODR^=Dial_DIR_PIN;
}

void Set_Point(uint32_t point)
{
    Point=point;
}

void Set_Dial_PWM(uint8_t PWM)
{
    TIM_SetCompare3(Dial_TIM,PWM);
}

void TIM8_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(Dial_TIM,TIM_IT_Update)==SET)
    {
        Point++;
        if (Point>Dial_One_round)
        {
            Set_Dial_PWM(0);
            // temp_Arm=1;
        }
        TIM_ClearITPendingBit(Dial_TIM,TIM_IT_Update);
    }
}

int8_t Min_square(int8_t *a)
{
    uint8_t i=0,j=0,k=0,q=0;
    int8_t arr[3]={0,0,0};
    int8_t tp1=0,tp2=0;
    for ( q = 0; q < 3; q++)
    {
        arr[q]=a[q];
    }
    if(Dial[2]==0){arr[0]=0;}
    if(Dial[5]==0){arr[1]=0;}
    if(Dial[7]==0){arr[2]=0;}
    for ( i = 1; i < 3; i++)
    {
        for ( j = 0; j < 3-i; j++)
        {
            if (fabs(arr[j])>fabs(arr[j+1]))
            {
                k=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=k;
            } 
        }
    }
    if(arr[0]==0)
    {
        if(arr[1]==0)
        {
            if(arr[2]==0){return 0;}
            else {return arr[2];}
        }
        else 
        {
            tp1=fabs(arr[1]);
            tp2=8-fabs(arr[2]);
            if(tp1<=tp2){return arr[1];}
            else return arr[2];
        }
    }
    else 
    {
        tp1=fabs(arr[0]);
        tp2=8-fabs(arr[2]);
        if(tp1<=tp2){return arr[0];}
        else return arr[2];
    }
}

int8_t Min_cylinder(int8_t *a)
{
    uint8_t i=0,j=0,k=0,q=0;
    int8_t arr[2]={0,0};
    int8_t tp1=0,tp2=0;
    for ( q = 0; q < 2; q++)
    {
        arr[q]=a[q];
    }
    if(Dial[0]==0){arr[0]=0;}
    if(Dial[4]==0){arr[1]=0;}
    for ( i = 1; i < 2; i++)
    {
        for ( j = 0; j < 2-i; j++)
        {
            if (fabs(arr[j])>fabs(arr[j+1]))
            {
                k=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=k;
            } 
        }
    }
    tp1=fabs(arr[0]);
    tp2=8-fabs(arr[1]);
    if(arr[0]==0)
    {
        if(Dial[0]==0){return arr[1];}
        else 
        {
            if(Dial[4]==0){return arr[1];}
            else {return arr[0];}
        }
    }
    else 
    {
        if(tp1<=tp2){return arr[0];}
        else return arr[1];
    }
}

int8_t Min_triangle(int8_t *a)
{
    uint8_t i=0,j=0,k=0,q=0;
    int8_t arr[3]={0,0,0};
    int8_t tp1=0,tp2=0;
    for ( q = 0; q < 3; q++)
    {
        arr[q]=a[q];
    }
    if(Dial[1]==0){arr[0]=0;}
    if(Dial[3]==0){arr[1]=0;}
    if(Dial[6]==0){arr[2]=0;}
    for ( i = 1; i < 3; i++)
    {
        for ( j = 0; j < 3-i; j++)
        {
            if (fabs(arr[j])>fabs(arr[j+1]))
            {
                k=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=k;
            } 
        }
    }
    if(arr[0]==0)
    {
        if(arr[1]==0)
        {
            if(arr[2]==0){return 0;}
            else {return arr[2];}
        }
        else 
        {
            tp1=fabs(arr[1]);
            tp2=8-fabs(arr[2]);
            if(tp1<=tp2){return arr[1];}
            else return arr[2];
        }
    }
    else 
    {
        tp1=fabs(arr[0]);
        tp2=8-fabs(arr[2]);
        if(tp1<=tp2){return arr[0];}
        else return arr[2];
    }
}

void Dial_State_Update(void)
{
    cylinder[0]=Dial_Now-cylinder1;cylinder[1]=Dial_Now-cylinder2;
    triangle[0]=Dial_Now-triangle1;triangle[1]=Dial_Now-triangle2;triangle[2]=Dial_Now-triangle3;
    square[0]=Dial_Now-square1;square[1]=Dial_Now-square2;square[2]=Dial_Now-square3;
}

void foreward(void)
{
    GPIO_ResetBits(Dial_PORT,Dial_DIR_PIN);
}

void reversal(void)
{
    GPIO_SetBits(Dial_PORT,Dial_DIR_PIN);
}