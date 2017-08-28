#include "common.h"
#include "include.h"

#define Pitch_P 400
#define Pitch_D 80
#define Roll_P 400
#define Roll_D 80

float Error = 0;
float LastError = 0;

void Pitch_Angle_Control()//motor1,2 y轴
{
  Error = Pitch_Angle - Target_Pitch_Angle;//得到偏差
  int32 PWM_angle_OutPut = (int32)(Pitch_P*Error+Pitch_D*(Error-LastError));//PID
  
  //OutPut_limit(PWM_angle_OutPut);//输出限幅
  if(abs(PWM_angle_OutPut)>TOP)
    PWM_angle_OutPut=TOP;

  PWM_angle_OutPut = abs(PWM_angle_OutPut);
  
  if(Error > 0)
  {
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,PWM_angle_OutPut); 
  }
  else
  {
    ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,PWM_angle_OutPut);
    ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);
  }
  
  LastError = Error;
}

void Roll_Angle_Control()//motor3,4 x轴
{
  Error = Roll_Angle - Target_Roll_Angle;
  
  int32 PWM_angle_OutPut = (int32)(Roll_P*Error+Roll_D*(Error-LastError));//PID
  
  //OutPut_limit(PWM_angle_OutPut);//输出限幅
  if(abs(PWM_angle_OutPut)>TOP)
    PWM_angle_OutPut=TOP;

  PWM_angle_OutPut = abs(PWM_angle_OutPut);
  
  if(Error > 0)
  {
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,0);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,PWM_angle_OutPut); 
  }
  else
  {
    ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,PWM_angle_OutPut);
    ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
  }
}
