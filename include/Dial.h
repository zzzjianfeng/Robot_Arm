#ifndef _Dial_H
#define _Dial_H

#include "stm32f10x.h"
#include <math.h>

extern uint32_t Point;  /*�������������*/
extern uint8_t Dial[8]; /*ת�̸����λ��״̬*/
extern uint8_t Dial_Now; 
extern int8_t triangle[3]; /*����λ������*/
extern int8_t cylinder[2]; /*Բ��λ������*/
extern int8_t square[3];   /*������λ������*/
extern uint32_t Arm2;  /*�������������*/

/*ת�̽ӿڶ���*/
#define Dial_PORT GPIOC
#define Dial_EN_PIN  GPIO_Pin_4    //ʹ�ܶ�
#define Dial_DIR_PIN GPIO_Pin_5    //���Ʒ���
#define Dial_STEP_PIN GPIO_Pin_8   //���Ʋ���
#define Dial_CLK RCC_APB2Periph_GPIOC
#define Dial_TIM TIM8
#define Dial_TIM_CLK RCC_APB2Periph_TIM8
#define Dial_One_round 128000/*3200*/   /*һȦ����������*/
#define Point_45(x) (Dial_One_round*(8-(x)))/8   /*�Ƕȼ��� 45�ȵı��� ע���������ڸ���������*/
/*ת�����λ�ö���*/    
#define cylinder1 0
#define cylinder2 4
#define triangle1 2
#define triangle2 5
#define triangle3 7
#define square1 1
#define square2 3
#define square3 6
/*ת������ת����*/
#define Rel 1
#define Fwd 0
/*ת�̹��ܺ�������*/
void Dial_Init(uint16_t arr,uint16_t psc);  /*ת�̳�ʼ��*/
void Dial_TOGGLE(void); /*ת�̷�ת����*/
void Set_Point(uint32_t point); /*����������*/
void Set_Dial_PWM(uint8_t PWM); /*����ռ�ձ�*/
int8_t Min_cylinder(int8_t *a); /*���㵽��Բ��·����Сֵ*/
int8_t Min_triangle(int8_t *a); /*���㵽������·����Сֵ*/
int8_t Min_square(int8_t *a);   /*���㵽��������·����Сֵ*/
void Dial_State_Update(void);
void foreward(void);    /*��ת*/
void reversal(void);    /*��ת*/

#endif
