/********************板球控制系统********************
            版本号：V2.1    路径优化
              苏晨珲  林本丰  左冲
******************************************************/

#include "common.h"
#include "include.h"

//函数声明
void main_menu_task(void);      //菜单函数

uint8  var1,var2;
uint16  var3,var4;
uint32 var5, var6,var7,var8,var9;

uint8 menu_select=8; //记录选择的菜单的哪个选项
/****************按键选项7置1返回***********************/
uint8 return_menu1=0; //选取第几个选项标志
uint8 return_menu2=0;//选定选项后 置1返回标志
/****************按键选项8置1写入***********************/
uint8 fw_confirm1=0; //flashwrite_confirm 选取第几个选项标志
uint8 fw_confirm2=0; //flashwrite_confirm 选定选项后 置1返回标志
/******************按键选项数记录,flash保存动态调参**********************/
uint8 memoryflag1=0;

uint8 memory_flag1=0;
uint8 memory_flag2=0;
uint8 memory_flag3=0;
uint8 memory_flag4=0;
uint8 memory_flag5=0;
uint8 memory_flag6=0;
uint8 memory_flag9=0;

uint8 memory11=0,memory12=0;
uint16 memory13=0,memory14=0;
uint32 memory15=0,memory16=0,memory19=0;

uint8 memory21=0,memory22=0;
uint16 memory23=0,memory24=0;
uint32 memory25=0,memory26=0,memory29=0;

uint8 memory31=0,memory32=0;
uint16 memory33=0,memory34=0;
uint32 memory35=0,memory36=0,memory39=0;

uint8 memory41=0,memory42=0;
uint16 memory43=0,memory44=0;
uint32 memory45=0,memory46=0,memory49=0;

uint8 memory51=0,memory52=0;
uint16 memory53=0,memory54=0;
uint32 memory55=0,memory56=0,memory59=0;

uint8 write_flag=1;

/*****************PID动态参数调节****************/
float kp = 54;
float kd = 47;
uint8 allow_change = 0;
/***********************************************/

void  main(void)
{
 
    flash_init();                    //初始化flash
    led_init (LED0);
    led_init (LED1);

    /********电机初始化**********/
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //初始化 电机 PWM  

    main_menu_task();  //菜单
    
}
