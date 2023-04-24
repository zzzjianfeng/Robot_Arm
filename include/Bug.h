#ifndef _BUG_H
#define _BUG_H

#include "stm32f10x.h"
#define KEY_GPIO_PIN    GPIO_Pin_0
#define KEY_ON  1
#define KEY_OFF 0
#define GPIO_PORT   GPIOA
#define KEY_GPIO_CLK   RCC_APB2Periph_GPIOA

unsigned char Bug(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);

#endif