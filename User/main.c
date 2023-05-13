#include "main.h"

uint8_t Dial_Now=0;   /*用于存储转盘当前位置*/
uint8_t Dial[8]={1,1,1,1,1,1,1,1};  /*转盘8个位置状态*/
int8_t triangle[3]={0,0,0}; /*三角位置数组*/
int8_t cylinder[2]={0,0}; /*圆柱位置数组*/
int8_t square[3]={0,0,0};   /*正方体位置数组*/
int8_t cylinder_min=0,triangle_min=0,square_min=0,tp=0; /*用于存储物块移动路径最小路径*/
/* 用于机械臂和转盘关联的变量 */
uint8_t temp_Arm=0;         
uint8_t temp_Arm_cylinder=0;
uint8_t temp_Arm_square=0;
uint8_t temp_Arm_triangle=0;

uint8_t eye=0;/* 用于判断是否正确接收到Openmv的指令 */

uint8_t eye2=0;/* 用于判断是否正确放入 */
  
int main()
{
    // OLED_Init();
    KEY_GPIO_Config();
    Usart_Init();
    delay_init();
    Conveyor_INIT();
    EXTI_INIT();
    // EXTI_INIT2();
    Servo_Init();
    Dial_Init(100,72);
	limit_switch_init();
	TIM1_PWM_Init();
    while (Key_Scan(KEY_GPIO_PORT,KEY_GPIO_PIN)==KEY_ON)
    {
        /* code */
    }
    MOVE_INIT3();
    Conveyor_ON();
    while(1)
    {
        // MOVE_INIT();
        // MOVE_1_();
        // MOVE_INIT();
        // MOVE_2_();
        // MOVE_INIT();
        // MOVE_3_();
        init:
        /* 机械臂动作 */
        if (temp_Arm==1)
        {
            MOVE_INIT3();
            if (temp_Arm_square==1)
            {MOVE_3_();eye2=1;}
            else if (temp_Arm_triangle==1)
            {MOVE_1_();eye2=1;}
            else if (temp_Arm_cylinder==1)
            {MOVE_2_();eye2=1;}
            temp_Arm=0;temp_Arm_square=0;temp_Arm_triangle=0;temp_Arm_cylinder=0;
            Conveyor_ON();
        }
        /* 转盘动作 */
        if (RXD_ReceiveFlag==1)
        {
            Dial_State_Update();
            if(Dial[0]==0&&Dial[1]==0&&Dial[2]==0&&Dial[3]==0&&Dial[4]==0&&Dial[5]==0&&Dial[6]==0&&Dial[7]==0) /*清零*/
            {
                for ( tp = 0; tp < 8; tp++)
                {
                    Dial[tp]=1;
                }
            }
            if (strcmp(RxPacket,"1")==0)
            {
                cylinder_min=Min_cylinder(cylinder);
                if(cylinder_min>0)
                {
                    reversal();
                    tp=8-cylinder_min;
                    if (tp<cylinder_min){foreward();Point=Point_45(tp);}
                    else {Point=Point_45(cylinder_min);}
                }
                else if(cylinder_min<0)
                {
                    foreward();
                    tp=8+cylinder_min;
                    if (tp<(-cylinder_min)){reversal();Point=Point_45(tp);}
                    else {Point=Point_45(-cylinder_min);}
                }
                if ((Dial[cylinder1]!=0)||(Dial[cylinder2]!=0))
                {
                    temp_Arm_cylinder=1;
                }
                Dial_Now=Dial_Now-cylinder_min;
                Dial[Dial_Now]=0;
                if (cylinder_min==0)
                {
                    goto first;        //如果第一个物件是圆就不动
                }
            }
            else if (strcmp(RxPacket,"3")==0)
            {
                triangle_min=Min_triangle(triangle);
                if(triangle_min>0)
                {
                    reversal();
                    tp=8-triangle_min;
                    if (tp<triangle_min){foreward();Point=Point_45(tp);}
                    else {Point=Point_45(triangle_min);}
                }
                else if(triangle_min<0)
                {
                    foreward();
                    tp=8+triangle_min;
                    if (tp<(-triangle_min)){reversal();Point=Point_45(tp);}
                    else {Point=Point_45(-triangle_min);}
                }
                if ((Dial[triangle1]!=0)||(Dial[triangle2]!=0)||(Dial[triangle3]!=0))
                {
                    temp_Arm_triangle=1;
                }
                Dial_Now=Dial_Now-triangle_min;
                Dial[Dial_Now]=0;
            }
            else if (strcmp(RxPacket,"2")==0)
            {
                square_min=Min_square(square);
                if(square_min>0)
                {
                    reversal();
                    tp=8-square_min;
                    if (tp<square_min){foreward();Point=Point_45(tp);}
                    else {Point=Point_45(square_min);}
                }
                else if(square_min<0)
                {
                    foreward();
                    tp=8+square_min;
                    if (tp<(-square_min)){reversal();Point=Point_45(tp);}
                    else {Point=Point_45(-square_min);}
                }
                if ((Dial[square1]!=0)||(Dial[square2]!=0)||(Dial[square3]!=0))
                {
                    temp_Arm_square=1;
                }
                Dial_Now=Dial_Now-square_min;
                Dial[Dial_Now]=0;
            }
            else goto init;
            Set_Dial_PWM(50);
            first: eye=1;
            RXD_ReceiveFlag=0;
        }
    }
}
