#include "common.h"
#include "pid.h"
#include "include.h"

/********* P I D 参数 PWM***********/
float Pwm_Px=400;//电机x
float Pwm_Ix=0;
float Pwm_Dx=80;

float Pwm_Py=400;//电机y
float Pwm_Iy=0;
float Pwm_Dy=80;

/********* P I D 参数 Angle**** P-0.052 D0.4*******/
float Angle_Px=-0.06;//电机x
float Angle_Ix=0;
float Angle_Dx=-1.3;

float Angle_Py=-0.035;//电机y
float Angle_Iy=0;
float Angle_Dy=-1.2;



/**********************************************************************************************
                         X轴上的PID(位置式)-PD控制 输出目标角度
                             微分先行+不完全微分+死区控制
**********************************************************************************************/
float X_PID_angle(int32 measured,int32 target)
{ 
  
    float result;//函数返回值
    float K=0.5;	//不完全微分系数
    extern float kp;extern float kd;extern uint8 allow_change;

   
    if(allow_change == 1)
    {
      Angle_Px = 0 - kp/1000;Angle_Dx = 0 - kd/100;
    }

    float KdWork;
    float bias;//本次偏差
    static float bias_last=0;//上次偏差
    static float bias_integration=0;//偏差积分
    static float KdWorkBbk=0;   //上次Kd项
    static float measured_last=0;   //微分先行项
    static int16 num=0;

    bias=measured-target;//计算偏差
    KdWork = Angle_Dx*K*(measured-measured_last)+ (1-K)*KdWorkBbk;	//计算不完全微分
    

    if ((abs(bias)<=6)&&(abs(bias_last)<=6))
    {
      if(num<50)  num++;
      if (num>=50)
      {
        led_turn (LED0);
        result=0;
      }
      else
      {
        result=(float)( Angle_Px*bias+KdWork); 
      }

    }
    else
    {
      num=0;
      result=(float)( Angle_Px*bias+KdWork); 
    }
    
    bias_integration+=bias;//对偏差积分
    bias_last=bias;//更新偏差值
    KdWorkBbk=KdWork;//更新Pd项
    measured_last=measured;//更新微分先行项
    
    
/**************角度限幅与小数点确定***************/
   result = (int32)(result*10);//翻滚面角度
   result = result/10;
    
    if(result<-10)
    {
          result=-10;
    }
    if(result>10)
    {
          result=10;
    }
    return result;
  
  
}

/**********************************************************************************************
                                    X角度上的PID(位置式)-PD控制 输出目标PWM
**********************************************************************************************/
int32 X_PID_PWM(float measured,float target)
{ 
    int32 result;//函数返回值

    int32 bias;//本次偏差
    static int32 bias_last=0;//上次偏差
    static int32 bias_integration=0;//偏差积分
    
    bias=(int32)measured-target;//计算偏差
    //bias_integration+=bias;//对偏差积分
    result=(int32)( Pwm_Px*bias+Pwm_Dx*(bias-bias_last));  
    bias_last=bias;//更新偏差值

    return result;
  
}

/**********************************************************************************************
                         Y轴上的PID(位置式)-PD控制 输出目标角度
                             微分先行+不完全微分+死区控制
**********************************************************************************************/
float Y_PID_angle(int32 measured,int32 target)
{ 
    float result;//函数返回值
    float K=0.5;	//不完全微分系数
    extern float kp;extern float kd;extern uint8 allow_change;
    
    if(allow_change == 1)
    {
        Angle_Py = 0 - kp/1000;Angle_Dy = 0 - kd/100;
    }
    
    float KdWork;
    float bias;//本次偏差
    static float bias_last=0;//上次偏差
    static float bias_integration=0;//偏差积分
    static float KdWorkBbk=0;   //上次Kd项
    static float measured_last=0;   //微分先行项
    static int16 num=0;
    
    bias=measured-target;//计算偏差
    KdWork = Angle_Dy*K*(measured-measured_last)+ (1-K)*KdWorkBbk;	//计算不完全微分+微分先行
    if ((abs(bias)<=6)&&(abs(bias_last)<=6))
    {
      if(num<50)  num++;
      if (num>=50)
      {
        led_turn (LED1);
        result=0;
      }
      else
      {
        result=(float)( Angle_Py*bias+KdWork); 
      }

    }
    else
    {
      num=0;
      result=(float)( Angle_Py*bias+KdWork); 
    }
    bias_integration+=bias;//对偏差积分
    bias_last=bias;//更新偏差值
    KdWorkBbk=KdWork;//更新Pd项
    measured_last=measured;//更新微分先行项
    

    
/**************角度限幅与小数点确定***************/
    result = (int32)(result*10);//翻滚面角度
    result = result/10;
    if(result<-10)
    {
          result=-10;
    }
    if(result>10)
    {
          result=10;
    }
    return result;
}

/**********************************************************************************************
                                    Y角度上的PID(位置式)-PD控制 输出目标PWM
**********************************************************************************************/
int32 Y_PID_PWM(float measured,float target)
{ 
    int32 result;//函数返回值

    float bias;//本次偏差
    static float bias_last=0;//上次偏差
    static float bias_integration=0;//偏差积分
    
    bias=measured-target;//计算偏差
    //bias_integration+=bias;//对偏差积分
    result=(int32)( Pwm_Py*bias+Pwm_Dy*(bias-bias_last) );
    bias_last=bias;//更新偏差值

    return result;
}


/**********************************************************************************************
                          X轴画圆的PID(位置式)-PD控制 输出目标角度
**********************************************************************************************/
float X_PID_Circle(int32 measured,int32 target)
{ 
    float result;//函数返回值

    extern float kp;extern float kd;extern uint8 allow_change;
    
    if(allow_change == 1)
    {Angle_Px = 0 - kp/1000;Angle_Dx = 0 - kd/100;}
    else
    {Angle_Px = -0.054;Angle_Dx = -0.47;}
    
    float bias;//本次偏差
    static float bias_last=0;//上次偏差
    static float bias_integration=0;//偏差积分
    
    bias=measured-target;//计算偏差
    //bias_integration+=bias;//对偏差积分
    result=(float)( Angle_Px*bias+Angle_Dx*(bias-bias_last));  
    bias_last=bias;//更新偏差值
    //角度限幅
    
   result = (int32)(result*10);//翻滚面角度
   result = result/10;
    
    if(result<-10)
    {
          result=-10;
    }
    if(result>10)
    {
          result=10;
    }
    return result;
  
}
/**********************************************************************************************
                          Y轴画圆的PID(位置式)-PD控制 输出目标角度
**********************************************************************************************/
float Y_PID_Circle(int32 measured,int32 target)
{ 
    float result;//函数返回值
    extern float kp;extern float kd;extern uint8 allow_change;
    if(allow_change == 1)
    {Angle_Py = 0 - kp/1000;Angle_Dy = 0 - kd/100;}
    else
    {Angle_Px = -0.054;Angle_Dx = -0.47;}
    
    int32 bias;//本次偏差
    static int32 bias_last=0;//上次偏差
    static int32 bias_integration=0;//偏差积分
    
    bias=measured-target;//计算偏差
   // bias_integration+=bias;//对偏差积分
    result=(float)( Angle_Py*bias+Angle_Dy*(bias-bias_last) );
    bias_last=bias;//更新偏差值
    
    //角度限幅
    result = (int32)(result*10);//翻滚面角度
    result = result/10;
    if(result<-10)
    {
          result=-10;
    }
    if(result>10)
    {
          result=10;
    }
    return result;
}