#ifndef _USART_H
#define _USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define TXD_PIN GPIO_Pin_9
#define RXD_PIN GPIO_Pin_10
#define Usart_CLK RCC_APB2Periph_USART1
#define Usart_GPIO_CLK RCC_APB2Periph_GPIOA
#define Usart_Port GPIOA
#define Usart_Number USART1
#define Usart_IRQn USART1_IRQn

extern char RxPacket[];
extern uint8_t RXD_ReceiveFlag;

void Usart_Init(void);
void SendData(uint8_t Data);
void SendString(char *String);
void SendNumber(int32_t Number,uint8_t Counter);
void SendArray(int16_t *Data,uint16_t Counter);
void Send_DataPacket(void);

uint16_t POW(int16_t Number,uint8_t Counter);


#endif
