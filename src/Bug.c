#include "Bug.h"

unsigned char Bug(GPIO_TypeDef *GPIOx,unsigned int GPIO_PIN)
{
    if (GPIO_ReadInputDataBit(GPIOx,GPIO_PIN)==KEY_ON)
    {
        return KEY_ON;
    }
    else return KEY_OFF;
}
