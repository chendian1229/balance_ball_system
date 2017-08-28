#include "common.h"
#include "include.h"

uint8 allow_control_flag = 0;        //允许PID控制标志位

uint8 stop_Roll_flag = 0;      
uint8 stop_Pitch_flag = 0;

int32 Pid_position_x = 0;//PID控制实时位置
int32 Pid_position_y = 0;

int32 SetPosition_x = 150;//小球目标位置设定
int32 SetPosition_y = 150;

int32 TOP  =  800;       //两个电机的PWM的输出限幅
int32 DOWN = -800;

int32 PWM_OutPut_x = 0; //输出给电机x的值
int32 PWM_OutPut_y = 0; //输出给电机y的值

void  Platform_Control()
{  
    int32 filter_position_x=Pid_position_x;
    int32 filter_position_y=Pid_position_y;   
    
    extern uint8 target_mode;
    extern uint8 goto_nine_flag;
    extern uint8 stubbon;
    
    Digital_filtering(&filter_position_x,&filter_position_y);        //x,y坐标的一阶低通滤波
    /******位置偏差测定  得到目标角度*****/
    allow_control_flag=0;
    static float Angle_target_x=0,Angle_target_y=0;//输出目标角度
    
    /*********************画圆任务中，调用低通滤波***********************/
    if(((target_mode == 7)&&(goto_nine_flag == 0))||((target_mode == 8)&&(stubbon == 0)))
    {
      Pid_position_x = filter_position_x;
      Pid_position_y = filter_position_y;
    }
    /*******************************************************************/
    if((target_mode == 7)||(target_mode == 8))
    {
    /******角度PID，由坐标得出应该倾斜的角度*****Pid_position_x*/
    Angle_target_x = X_PID_Circle(Pid_position_x,SetPosition_x);
    Angle_target_y = Y_PID_Circle(Pid_position_y,SetPosition_y);
    }
    else
    {
    /******角度PID，由坐标得出应该倾斜的角度*****Pid_position_x*/
    Angle_target_x = X_PID_angle(Pid_position_x,SetPosition_x);
    Angle_target_y = Y_PID_angle(Pid_position_y,SetPosition_y);
    }
    
    /******位置PID，由角度得出电机PWM输入********/
    PWM_OutPut_x = X_PID_PWM(Roll_Angle,Angle_target_y+0.2);
    PWM_OutPut_y = Y_PID_PWM(Pitch_Angle,Angle_target_x-0.5);

  /***********输出限幅******************/
  if(PWM_OutPut_x>TOP)
    PWM_OutPut_x = TOP;
  if(PWM_OutPut_x<DOWN)
    PWM_OutPut_x = DOWN;
  if(PWM_OutPut_y>TOP)
    PWM_OutPut_y = TOP;
  if(PWM_OutPut_y<DOWN)
    PWM_OutPut_y = DOWN;
  
//  //小范围限幅，减少电机负担
//  if(abs(PWM_OutPut_x)<50)
//    PWM_OutPut_x=0;
//  if(abs(PWM_OutPut_y)<50)
//    PWM_OutPut_y=0;
  


  /***********控制X坐标轴******/

    if(PWM_OutPut_x>=0)//下降
    {
        ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,PWM_OutPut_x);
        
    }
    else //上升
    {
        PWM_OutPut_x=abs(PWM_OutPut_x);
        ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_OutPut_x);    
    }   

  /**********控制Y坐标轴************/

    if(PWM_OutPut_y>=0)//下降
    {
        ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,PWM_OutPut_y);
    }
    else //上升
    { 
        PWM_OutPut_y=abs(PWM_OutPut_y);
        ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);    
        ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,PWM_OutPut_y);
    } 
    
}
/******************一阶低通滤波*************/
void Digital_filtering(int32 *pos_x,int32 *pos_y)
{
    float K=0.5;                //把系数无脑给小，可按照目标角度画圆
    static int32 pos_x_last=0;
    static int32 pos_y_last=0;
    extern uint8 circle_flag;
    extern uint8 target_mode;
    
    if((target_mode == 8)&&(K < 0.9))
      K += 0.05;
    else if(K < 0.9)
      K = 0.5;
    
    *pos_y = (int32)(K*(*pos_y) + (1-K)*pos_y_last);
    *pos_x = (int32)(K*(*pos_x) + (1-K)*pos_x_last);
    pos_y_last=*pos_y;
    pos_x_last=*pos_x;
    
}

