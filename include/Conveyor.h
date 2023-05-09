#ifndef _Conveyor_H
#define _Conveyor_H

#include "stm32f10x.h"

/*Conveyor Define*/
#define Conveyor_PIN GPIO_Pin_6
#define Conveyor_CLK RCC_APB2Periph_GPIOD
#define Conveyor_Port GPIOD
/*Conveyor func*/
void Conveyor_INIT(void);
void Conveyor_ON(void);
void Conveyor_OFF(void);
void Conveyor_TOGGLE(void);

#endif
