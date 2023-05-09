#ifndef _Dial_H
#define _Dial_H

#include "stm32f10x.h"
#include <math.h>

extern uint32_t Point;  /*步进电机脉冲数*/
extern uint8_t Dial[8]; /*转盘各物件位置状态*/
extern uint8_t Dial_Now; 
extern int8_t triangle[3]; /*三角位置数组*/
extern int8_t cylinder[2]; /*圆柱位置数组*/
extern int8_t square[3];   /*正方体位置数组*/
extern uint32_t Arm2;  /*步进电机脉冲数*/

/*转盘接口定义*/
#define Dial_PORT GPIOC
#define Dial_EN_PIN  GPIO_Pin_4    //使能端
#define Dial_DIR_PIN GPIO_Pin_5    //控制方向
#define Dial_STEP_PIN GPIO_Pin_8   //控制步数
#define Dial_CLK RCC_APB2Periph_GPIOC
#define Dial_TIM TIM8
#define Dial_TIM_CLK RCC_APB2Periph_TIM8
#define Dial_One_round 128000/*3200*/   /*一圈所需脉冲数*/
#define Point_45(x) (Dial_One_round*(8-(x)))/8   /*角度计算 45度的倍数 注：不能用于浮点数计算*/
/*转盘物件位置定义*/    
#define cylinder1 0
#define cylinder2 4
#define triangle1 2
#define triangle2 5
#define triangle3 7
#define square1 1
#define square2 3
#define square3 6
/*转盘正反转定义*/
#define Rel 1
#define Fwd 0
/*转盘功能函数定义*/
void Dial_Init(uint16_t arr,uint16_t psc);  /*转盘初始化*/
void Dial_TOGGLE(void); /*转盘反转函数*/
void Set_Point(uint32_t point); /*设置脉冲数*/
void Set_Dial_PWM(uint8_t PWM); /*设置占空比*/
int8_t Min_cylinder(int8_t *a); /*计算到达圆柱路径最小值*/
int8_t Min_triangle(int8_t *a); /*计算到达三角路径最小值*/
int8_t Min_square(int8_t *a);   /*计算到达正方体路径最小值*/
void Dial_State_Update(void);
void foreward(void);    /*正转*/
void reversal(void);    /*反转*/

#endif
