#include "Conveyor.h"

void Conveyor_INIT(void)
{
    RCC_APB2PeriphClockCmd(Conveyor_CLK,ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin=Conveyor_PIN;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Conveyor_Port,&GPIO_InitStruct);

    GPIO_SetBits(Conveyor_Port,Conveyor_PIN);
}

void Conveyor_ON(void)
{
    GPIO_ResetBits(Conveyor_Port,Conveyor_PIN);
}

void Conveyor_OFF(void)
{
    GPIO_SetBits(Conveyor_Port,Conveyor_PIN);
}

void Conveyor_TOGGLE(void)
{
    Conveyor_Port->ODR^=Conveyor_PIN;
}
