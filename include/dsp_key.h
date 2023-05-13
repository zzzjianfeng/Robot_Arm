#ifndef _dsp_key_H
#define _dsp_key_H

#include "stm32f10x.h"
#define KEY_GPIO_PIN    GPIO_Pin_12
#define KEY_ON  1
#define KEY_OFF 0
#define KEY_GPIO_PORT   GPIOA
#define KEY_GPIO_CLK   RCC_APB2Periph_GPIOA

void KEY_GPIO_Config(void);
unsigned char Key_Scan(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN);

#endif
