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


/********** 驱动器 端口定义 **************
//DRIVER_DIR   PC0 
//DRIVER_OE    PC2 
//STEP_PULSE   PC7 (TIM8_CH2,LCD_RW)
******************************************/
// X轴电机相关引脚定义		丝杆
#define STEPMOTOR_TIM_CHANNEL1                TIM_Channel_1			  //定时器通道1//X轴
#define STEPMOTOR_TIM_IT_CC1                  TIM_IT_CC1			    //定时器通道1中断使能位
#define STEPMOTOR_TIM_FLAG_CC1                TIM_FLAG_CC1			  //定时器通道1中断标志位
#define STEPMOTOR_TIM_PULSE_PIN_X             GPIO_Pin_9/*GPIO_Pin_8*/        //输出脉冲给X轴电机控制器
#define STEPMOTOR_DRIVER_DIR1   PCout(1) // 旋转方向 
#define STEPMOTOR_DRIVER_EN1    PCout(0) // 使能脚 低电平有效
#define STEPMOTOR_DRIVER_DIR_PIN1     GPIO_Pin_1
#define STEPMOTOR_DRIVER_EN_PIN1     GPIO_Pin_0

#define STEP_PULSE_PORT GPIOE
#define STEP_PULSE_RCC RCC_APB2Periph_GPIOE

// Y轴电机相关引脚定义		从上往下数第二个电机
#define STEPMOTOR_TIM_CHANNEL2                TIM_Channel_2			            // 定时器通道2//Y轴
#define STEPMOTOR_TIM_IT_CC2                  TIM_IT_CC2			              // 定时器通道2中断使能位
#define STEPMOTOR_TIM_FLAG_CC2                TIM_FLAG_CC2                  // 定时器通道1中断标志位
#define STEPMOTOR_TIM_PULSE_PIN_Y             GPIO_Pin_11/*GPIO_Pin_9*/                    // 输出脉冲给Y轴电机控制器
#define STEPMOTOR_DRIVER_DIR2   PCout(3) // 旋转方向 
#define STEPMOTOR_DRIVER_EN2    PCout(2) // 使能脚 低电平有效
#define STEPMOTOR_DRIVER_DIR_PIN2     GPIO_Pin_3
#define STEPMOTOR_DRIVER_EN_PIN2     GPIO_Pin_2

// Z轴电机相关引脚定义    爪子
#define STEPMOTOR_TIM_CHANNEL3                TIM_Channel_3			            //定时器通道3//Z轴
#define STEPMOTOR_TIM_IT_CC3                  TIM_IT_CC3			
#define STEPMOTOR_TIM_FLAG_CC3                TIM_FLAG_CC3
#define STEPMOTOR_TIM_PULSE_PIN_Z             GPIO_Pin_13/*GPIO_Pin_10*/                   //输出脉冲给Z轴电机控制器
#define STEPMOTOR_DRIVER_DIR3    PCout(11)// 旋转方向 
#define STEPMOTOR_DRIVER_EN3    PCout(10) // 使能脚 低电平有效
#define STEPMOTOR_DRIVER_DIR_PIN3     GPIO_Pin_11
#define STEPMOTOR_DRIVER_EN_PIN3     GPIO_Pin_10

// R轴相关定义	转台
#define STEPMOTOR_TIM_CHANNEL4                TIM_Channel_4			            //定时器通道4//R轴
#define STEPMOTOR_TIM_IT_CC4                  TIM_IT_CC4	
#define STEPMOTOR_TIM_FLAG_CC4                TIM_FLAG_CC4
#define STEPMOTOR_TIM_PULSE_PIN_R             GPIO_Pin_14/*GPIO_Pin_11*/                   //输出脉冲给R轴电机控制器
#define STEPMOTOR_DRIVER_DIR4   PCout(13) // 旋转方向 
#define STEPMOTOR_DRIVER_EN4    PCout(12) // 使能脚 低电平有效
#define STEPMOTOR_DRIVER_DIR_PIN4     GPIO_Pin_13
#define STEPMOTOR_DRIVER_EN_PIN4     GPIO_Pin_12

//限位开关相关定义
#define stop_XL    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define stop_YU    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define stop_ZF    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define stop_F     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define stop(x)     GPIO_ReadInputDataBit(GPIOE,PEin(x))

#define CW                0 // 顺时针
#define CCW               1 // 逆时针

/*电机速度决策中的四个状态*/
#define STOP              0 // 停止状态
#define ACCEL             1 // 加速状态
#define DECEL             2 // 减速状态
#define RUN               3 // 匀速状态

/*频率相关参数*/
//定时器实际时钟频率为：72MHz/(TIM_PRESCALER+1)
//其中 高级定时器的 频率为72MHz,其他定时器为72MHz
//168/(5+1)=28Mhz
//具体需要的频率可以自己计算
#define T1_FREQ           2000000 // 频率ft值

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


