#ifndef _MG996R_H
#define _MG996R_H

#include "stm32f10x.h"

#define MG996R1_Port GPIOB
#define MG996R1_Pin GPIO_Pin_6
#define MG996R1_Tim TIM4
#define MG996R1_Tim_Clk RCC_APB1Periph_TIM4
#define MG996R1_Gpio_Clk RCC_APB2Periph_GPIOB

#define MG996R2_Port GPIOB
#define MG996R2_Pin GPIO_Pin_7
#define MG996R2_Tim TIM4
#define MG996R2_Tim_Clk RCC_APB1Periph_TIM4
#define MG996R2_Gpio_Clk RCC_APB2Periph_GPIOB

void MG996R1_Init(uint16_t arr,uint16_t psc);
void MG996R2_Init(uint16_t arr,uint16_t psc);
void Servo_Init(void);
void Servo_SetLength(u8 Servox, float Length);
void Servo_Run(u8 Mode);

#endif
