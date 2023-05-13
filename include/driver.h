#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"
#include "stdlib.h"	
#include "math.h"
#include "delay.h"
#include "math.h"
#include "Usart.h"
#include "OLED.h"
#include <string.h>
#include "MG996R.h"
#include "Conveyor.h"
#include "Usart.h"
#include "Conveyor.h"
#include "Dial.h"

typedef struct 
{
	uint8_t  run_state ; 
	uint8_t  dir ;    
	int  step_delay;  
	int  decel_start; 
	int  decel_val;   
	int  min_delay;   
	int  accel_count; 
}speedRampData;

typedef struct 
{
	uint8_t  left ; 
	uint8_t  right ;
}motorexit;


/********** ������ �˿ڶ��� **************
//DRIVER_DIR   PC0 
//DRIVER_OE    PC2 
//STEP_PULSE   PC7 (TIM8_CH2,LCD_RW)
******************************************/
// X����������Ŷ���		˿��
#define STEPMOTOR_TIM_CHANNEL1                TIM_Channel_1			  //��ʱ��ͨ��1//X��
#define STEPMOTOR_TIM_IT_CC1                  TIM_IT_CC1			    //��ʱ��ͨ��1�ж�ʹ��λ
#define STEPMOTOR_TIM_FLAG_CC1                TIM_FLAG_CC1			  //��ʱ��ͨ��1�жϱ�־λ
#define STEPMOTOR_TIM_PULSE_PIN_X             GPIO_Pin_9/*GPIO_Pin_8*/        //��������X����������
#define STEPMOTOR_DRIVER_DIR1   PCout(1) // ��ת���� 
#define STEPMOTOR_DRIVER_EN1    PCout(0) // ʹ�ܽ� �͵�ƽ��Ч
#define STEPMOTOR_DRIVER_DIR_PIN1     GPIO_Pin_1
#define STEPMOTOR_DRIVER_EN_PIN1     GPIO_Pin_0

#define STEP_PULSE_PORT GPIOE
#define STEP_PULSE_RCC RCC_APB2Periph_GPIOE

// Y����������Ŷ���		�����������ڶ������
#define STEPMOTOR_TIM_CHANNEL2                TIM_Channel_2			            // ��ʱ��ͨ��2//Y��
#define STEPMOTOR_TIM_IT_CC2                  TIM_IT_CC2			              // ��ʱ��ͨ��2�ж�ʹ��λ
#define STEPMOTOR_TIM_FLAG_CC2                TIM_FLAG_CC2                  // ��ʱ��ͨ��1�жϱ�־λ
#define STEPMOTOR_TIM_PULSE_PIN_Y             GPIO_Pin_11/*GPIO_Pin_9*/                    // ��������Y����������
#define STEPMOTOR_DRIVER_DIR2   PCout(3) // ��ת���� 
#define STEPMOTOR_DRIVER_EN2    PCout(2) // ʹ�ܽ� �͵�ƽ��Ч
#define STEPMOTOR_DRIVER_DIR_PIN2     GPIO_Pin_3
#define STEPMOTOR_DRIVER_EN_PIN2     GPIO_Pin_2

// Z����������Ŷ���    צ��
#define STEPMOTOR_TIM_CHANNEL3                TIM_Channel_3			            //��ʱ��ͨ��3//Z��
#define STEPMOTOR_TIM_IT_CC3                  TIM_IT_CC3			
#define STEPMOTOR_TIM_FLAG_CC3                TIM_FLAG_CC3
#define STEPMOTOR_TIM_PULSE_PIN_Z             GPIO_Pin_13/*GPIO_Pin_10*/                   //��������Z����������
#define STEPMOTOR_DRIVER_DIR3    PCout(11)// ��ת���� 
#define STEPMOTOR_DRIVER_EN3    PCout(10) // ʹ�ܽ� �͵�ƽ��Ч
#define STEPMOTOR_DRIVER_DIR_PIN3     GPIO_Pin_11
#define STEPMOTOR_DRIVER_EN_PIN3     GPIO_Pin_10

// R����ض���	ת̨
#define STEPMOTOR_TIM_CHANNEL4                TIM_Channel_4			            //��ʱ��ͨ��4//R��
#define STEPMOTOR_TIM_IT_CC4                  TIM_IT_CC4	
#define STEPMOTOR_TIM_FLAG_CC4                TIM_FLAG_CC4
#define STEPMOTOR_TIM_PULSE_PIN_R             GPIO_Pin_14/*GPIO_Pin_11*/                   //��������R����������
#define STEPMOTOR_DRIVER_DIR4   PCout(13) // ��ת���� 
#define STEPMOTOR_DRIVER_EN4    PCout(12) // ʹ�ܽ� �͵�ƽ��Ч
#define STEPMOTOR_DRIVER_DIR_PIN4     GPIO_Pin_13
#define STEPMOTOR_DRIVER_EN_PIN4     GPIO_Pin_12

//��λ������ض���
#define stop_XL    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define stop_YU    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define stop_ZF    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define stop_F     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define stop(x)     GPIO_ReadInputDataBit(GPIOE,PEin(x))

#define CW                0 // ˳ʱ��
#define CCW               1 // ��ʱ��

/*����ٶȾ����е��ĸ�״̬*/
#define STOP              0 // ֹͣ״̬
#define ACCEL             1 // ����״̬
#define DECEL             2 // ����״̬
#define RUN               3 // ����״̬

/*Ƶ����ز���*/
//��ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/(TIM_PRESCALER+1)
//���� �߼���ʱ���� Ƶ��Ϊ72MHz,������ʱ��Ϊ72MHz
//168/(5+1)=28Mhz
//������Ҫ��Ƶ�ʿ����Լ�����
#define T1_FREQ           2000000 // Ƶ��ftֵ

void limit_switch_init(void);
void AxisMove(int32_t Axis,int32_t step, uint32_t accel, uint32_t decel, int32_t speed);
void TIM1_PWM_Init(void);
void timopen(int8_t Axis);
void timdisable(int8_t Axis);
void limitswitch(void);
void Step_Motor_Contrl(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num);
void Step_Motor_Contrl_Alone(uint8_t Num,uint32_t accel,uint32_t decel,int32_t speed,int32_t pulse_num);
void TIM1_CC_IRQHandler(void);
void Step_Motor_Init(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num);
void MOVE_INIT(void);
void MOVE_1_(void);	
void MOVE_2_(void);	
void MOVE_3_(void);
void EXTI_INIT(void);
void EXTI_INIT2(void);
void Step_Motor_Contrl2(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num);

uint8_t limitsw_Num(uint8_t Num);
void Step_Motor_Init_Alone(uint8_t Num,uint32_t accel,uint32_t decel,int32_t speed,int32_t pulse_num);
void MOVE_INIT2(void);
void MOVE_INIT3(void);
void Step_Motor_Contrl1_4(uint32_t accel,uint32_t decel,int32_t speed,int32_t *pulse_num);

#endif


