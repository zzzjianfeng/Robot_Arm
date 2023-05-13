#include "dsp_key.h"

/*按键初始化*/
void KEY_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(KEY_GPIO_CLK,ENABLE);
    GPIO_InitStruct.GPIO_Pin=KEY_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(KEY_GPIO_PORT,&GPIO_InitStruct);
    
}
/*按键判断*/
unsigned char Key_Scan(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN)
{
    if (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==KEY_ON)
    {
        while (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==KEY_ON);
        return KEY_ON;
    }
    else return KEY_OFF;
}
