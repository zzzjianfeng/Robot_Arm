#include "Usart.h"

int16_t TxPacket[4];
uint8_t RXD_ReceiveFlag;
uint8_t RXD_ReceiveData;
char RxPacket[100];

void Usart_Init(void)
{
    RCC_APB2PeriphClockCmd(Usart_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(Usart_GPIO_CLK,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=TXD_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Usart_Port,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=RXD_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(Usart_Port,&GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=9600;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_StopBits=USART_StopBits_1;
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_Init(Usart_Number,&USART_InitStructure);

    USART_ITConfig(Usart_Number,USART_IT_RXNE,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=Usart_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(Usart_Number,ENABLE);
}

void SendData(uint8_t Data)
{
    USART_SendData(Usart_Number,Data);
    while (USART_GetFlagStatus(Usart_Number,USART_FLAG_TXE)==RESET);
}

void SendString(char *String)
{
    uint8_t i;
    for ( i = 0;String[i]!='\0'; i++)
    {
        SendData(String[i]);
    }
}

uint16_t POW(int16_t Number,uint8_t Counter)
{
    uint8_t i;
    uint16_t temp=1;
    for ( i = Counter; i > 0; i--)
    {
        temp*=Number;
    }
    return temp;
}

void SendNumber(int32_t Number,uint8_t Counter)
{
    uint8_t i;
    if (Number<0)
    {
        SendData('-');
        for ( i = Counter; i > 0; i--)
        {
            if (i==1)
            {
                SendData((-Number)%10+'0');
            }
            else SendData(((-Number)/POW(10,i-1))%10+'0');
        }
    }
    else for ( i = Counter; i > 0; i--)
    {
        if (i==1)
        {
            SendData(Number%10+'0');
        }
        else SendData((Number/POW(10,i-1))%10+'0');
    }
    SendData('\n');
}

void SendArray(int16_t *Data,uint16_t Counter)
{
    uint8_t i;
    for ( i = 0; i < Counter; i++)
    {
        SendData(Data[i]);
    }
}

void Send_DataPacket(void)
{
    SendData(0xFF);
    SendArray(TxPacket,4);
    SendData(0xFE);
}

int fputc(int ch, FILE *f)
{
 	 SendData(ch);
   return ch;
}

void USART1_IRQHandler(void)
{    
   if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
   {
        RxPacket[0]=USART_ReceiveData(USART1);
        RXD_ReceiveFlag=1;
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
   }
}

// void USART1_IRQHandler(void)
// {    
//     uint8_t RxData=USART_ReceiveData(Usart_Number);
//    if(USART_GetITStatus(Usart_Number,USART_IT_RXNE)==SET)
//    {
//         if (RXD_ReceiveFlag==0)
//         {
//             RxPacket[0]=RxData;
//             RxPacket[1]='\0';
//             RXD_ReceiveFlag=1;
//         }     
//     USART_ClearITPendingBit(Usart_Number,USART_IT_RXNE);
//    }
// }
