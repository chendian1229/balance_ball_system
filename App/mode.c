#include "common.h"
#include "include.h"
    
   /***********测试点坐标***************/
  
  uint8 A=1,B=1,C=1,D=1;
  
  int32 SetPosition_temp_x1=31; 
  int32 SetPosition_temp_y1=29;
  
  int32 SetPosition_temp_x2=97;
  int32 SetPosition_temp_y2=27;
  
  int32 SetPosition_temp_x3=164;
  int32 SetPosition_temp_y3=29;
  
  int32 SetPosition_temp_x4=20;
  int32 SetPosition_temp_y4=97;
  
  int32 SetPosition_temp_x5=97;
  int32 SetPosition_temp_y5=97;
  
  int32 SetPosition_temp_x6=166;
  int32 SetPosition_temp_y6=97;
  
  int32 SetPosition_temp_x7=29;
  int32 SetPosition_temp_y7=163;
  
  int32 SetPosition_temp_x8=96;
  int32 SetPosition_temp_y8=166;
  
  int32 SetPosition_temp_x9=167;
  int32 SetPosition_temp_y9=164;
  
  /*************************************/ 

uint8 target_mode=1;
uint8 goto_nine_flag = 0;
uint8 stubbon = 0;
/***************************************************
              ABCD四个指定目标点函数
***************************************************/

int32 ABCD_judge(uint8 mod)
{
  extern int32 SetPosition_x;
  extern int32 SetPosition_y;
  if(mod == 1)
  {SetPosition_x = SetPosition_temp_x1;SetPosition_y = SetPosition_temp_y1;}
  else if(mod == 2)
  {SetPosition_x = SetPosition_temp_x2;SetPosition_y = SetPosition_temp_y2;}
  else if(mod == 3)
  {SetPosition_x = SetPosition_temp_x3;SetPosition_y = SetPosition_temp_y3;}
  else if(mod == 4)
  {SetPosition_x = SetPosition_temp_x4;SetPosition_y = SetPosition_temp_y4;}
  else if(mod == 5)
  {SetPosition_x = SetPosition_temp_x5;SetPosition_y = SetPosition_temp_y5;}
  else if(mod == 6)
  {SetPosition_x = SetPosition_temp_x6;SetPosition_y = SetPosition_temp_y6;}
  else if(mod == 7)
  {SetPosition_x = SetPosition_temp_x7;SetPosition_y = SetPosition_temp_y7;}
  else if(mod == 8)
  {SetPosition_x = SetPosition_temp_x8;SetPosition_y = SetPosition_temp_y8;}
  else if(mod == 9)
  {SetPosition_x = SetPosition_temp_x9;SetPosition_y = SetPosition_temp_y9;}

}
/***************************************************
                   模式选择函数
***************************************************/
void mode_select()
{
  
  uint8 SetBias = 8;
  static int16 flag_jishu = 0;
  extern int32 SetPosition_x;
  extern int32 SetPosition_y;
  extern float Angle_Px;extern float Angle_Py;
  extern float Angle_Dx;extern float Angle_Dy;
  
 /**********进入任务一 定点区域二***************************************/ 
  if(target_mode == 1)
    {
      Roll_Angle_Control(); //6050控制程序
      Pitch_Angle_Control();
    }
 
 /**********进入任务二 区域一进入区域五********************************/  
  else if(target_mode == 2)
    {
         SetPosition_x= SetPosition_temp_x5;
         SetPosition_y =SetPosition_temp_y5;
    }
 
  /**********进入任务三 直角走线，区域一进入区域五**********************/  
  else if(target_mode == 3)
    {
     if(flag_jishu < 250)//未抵达第一目标前
     {
       SetBias = 10;
       SetPosition_x = SetPosition_temp_x4;//给予第一目标值
       SetPosition_y = SetPosition_temp_y4;
       if((Pid_position_x > SetPosition_x)&&(abs(Pid_position_x-SetPosition_x)>SetBias-3)&&(abs(Pid_position_x-SetPosition_x)<SetBias+6)&&(abs(Pid_position_y-SetPosition_y)<SetBias+6))
         flag_jishu++;//小偏差时计时间
       else
         flag_jishu=0;//否则立刻清零
     }
     else//抵达超过2秒后，改变目标值
     {
       SetPosition_x = SetPosition_temp_x5;//给予第二目标值
       SetPosition_y = SetPosition_temp_y5;
     }
   }
    
 /**********进入任务四 无碰撞区域一进入区域九**************************/ 
  else if(target_mode == 4)
    {
      static uint16 flag_jishu1=0,flag_jishu2=0;
     if(flag_jishu1 < 100)//未抵达第一目标前
     {
       SetPosition_x = SetPosition_temp_x2;//给予第1目标值
       SetPosition_y = (SetPosition_temp_y2+SetPosition_temp_y5)/2;
       if((abs(Pid_position_x-SetPosition_x)<SetBias+6)&&(abs(Pid_position_y-SetPosition_y)<SetBias+6))
         flag_jishu1++;//小偏差时计时间
       else
         flag_jishu1=0;//否则立刻清零
     }
     else if((flag_jishu1 < 200)&&(flag_jishu1 >= 100))
      {
       SetPosition_x = (SetPosition_temp_x2+SetPosition_temp_x3)/2;//给予第2目标值
       SetPosition_y = (SetPosition_temp_y2+SetPosition_temp_y5)/2;
       if((abs(Pid_position_x-SetPosition_x)<SetBias+6)&&(abs(Pid_position_y-SetPosition_y)<SetBias+6))
         flag_jishu1++;//小偏差时计时间
       else
         flag_jishu1=100;//否则立刻清零
     }
     else if(flag_jishu2 < 100)//未抵达第一目标前
     {
       SetPosition_x = (SetPosition_temp_x2+SetPosition_temp_x3)/2;//给予第3目标值
       SetPosition_y = SetPosition_temp_y5;
       if((abs(Pid_position_x-SetPosition_x)<SetBias+6)&&(abs(Pid_position_y-SetPosition_y)<SetBias+6))
         flag_jishu2++;//小偏差时计时间
       else
         flag_jishu2=0;//否则立刻清零
     }
     else if((flag_jishu2 < 200)&&(flag_jishu2 >= 100))
      {
       SetPosition_x = (SetPosition_temp_x2+SetPosition_temp_x3)/2+10;//给予第4目标值
       SetPosition_y = (SetPosition_temp_y5+SetPosition_temp_y9)/2+10;
       if((abs(Pid_position_x-SetPosition_x)<SetBias+6)&&(abs(Pid_position_y-SetPosition_y)<SetBias+6))
         flag_jishu2++;//小偏差时计时间
       else
         flag_jishu2=100;//否则立刻清零
     }
     else
     {
       SetPosition_x = SetPosition_temp_x9;//给予第三目标值
       SetPosition_y = SetPosition_temp_y9;
     }
    }
    
 /**********进入任务五 区域一进入区域二，再进入区域六，再进入区域九****/ 
  else if(target_mode == 5)
    {
       static uint32 flag_jishu1=0,flag_jishu2=0;
     if(flag_jishu1 < 30)//未抵达第一目标前
     {
       SetPosition_x = SetPosition_temp_x2;//给予第1目标值
       SetPosition_y = SetPosition_temp_y2;
       if((abs(Pid_position_x-SetPosition_x)<SetBias-2)&&(abs(Pid_position_y-SetPosition_y)<SetBias-2))
         flag_jishu1++;//小偏差时计时间
       else
         flag_jishu1=0;//否则立刻清零
     }
     else if((flag_jishu1 < 430)&&(flag_jishu1 >= 30))
     {
       SetPosition_x = (SetPosition_temp_x2+SetPosition_temp_x3)/2;//给予第2目标值
       SetPosition_y = (SetPosition_temp_y2+SetPosition_temp_y5)/2;
         flag_jishu1++;//小偏差时计时间
     }
     else if(flag_jishu2 < 300)
     {
       SetPosition_x = SetPosition_temp_x3;//给予第3目标值
       SetPosition_y = (SetPosition_temp_y2+SetPosition_temp_y5)/2;
         flag_jishu2++;//小偏差时计时间
     }
     else if((flag_jishu2 < 330)&&(flag_jishu2 >= 300))
     {
       SetPosition_x = SetPosition_temp_x6;//给予第4目标值
       SetPosition_y = SetPosition_temp_y6;
       if((abs(Pid_position_x-SetPosition_x)<SetBias-2)&&(abs(Pid_position_y-SetPosition_y)<SetBias-2))
         flag_jishu2++;//小偏差时计时间
       else
         flag_jishu2=300;//否则立刻清零
     }
     else if((flag_jishu2 < 630)&&(flag_jishu2 >= 330))
     {
       SetPosition_x = SetPosition_temp_x9;//给予第5目标值
       SetPosition_y = (SetPosition_temp_y6+SetPosition_temp_y9)/2;
         flag_jishu2++;//小偏差时计时间
     }
     else
     {
       SetPosition_x = SetPosition_temp_x9;
       SetPosition_y = SetPosition_temp_y9;
     }
    }
   
 /**********进入区域六 ABCD定点走线************************************/ 
  else if(target_mode == 6)
    {
     static uint16 flag_jishu1=0,flag_jishu2=0,flag_jishu3=0,flag_jishu4=0;
      if(flag_jishu1 < 200)
      {
        ABCD_judge(A); 
        uint8 ram_x = SetPosition_x,ram_y=SetPosition_y;
        ABCD_judge(B);
        SetPosition_x = (ram_x + SetPosition_x)/2;
        SetPosition_y = (ram_y + SetPosition_y)/2;
        flag_jishu1++;
      }
      else if(flag_jishu2 < 60)
      {
        ABCD_judge(B);
        if((abs(Pid_position_x-SetPosition_x)<SetBias+3)&&(abs(Pid_position_y-SetPosition_y)<SetBias+3))
         flag_jishu2++;//小偏差时计时间
       else
         flag_jishu2=0;//否则立刻清零
      }
      else if((flag_jishu2 >= 60)&&(flag_jishu2 < 200))
      {
        ABCD_judge(B); 
        uint8 ram_x = SetPosition_x,ram_y=SetPosition_y;
        ABCD_judge(C);
        SetPosition_x = (ram_x + SetPosition_x)/2;
        SetPosition_y = (ram_y + SetPosition_y)/2;
        flag_jishu2++;
      }
      else if(flag_jishu3 < 60)
      {
        ABCD_judge(C);
        if((abs(Pid_position_x-SetPosition_x)<SetBias+3)&&(abs(Pid_position_y-SetPosition_y)<SetBias+3))
         flag_jishu3++;//小偏差时计时间
       else
         flag_jishu3=0;//否则立刻清零
      }
      else if((flag_jishu3 >= 60)&&(flag_jishu3 < 200))
      {
        ABCD_judge(D); 
        uint8 ram_x = SetPosition_x,ram_y=SetPosition_y;
        ABCD_judge(C);
        SetPosition_x = (ram_x + SetPosition_x)/2;
        SetPosition_y = (ram_y + SetPosition_y)/2;
        flag_jishu3++;
      }
      else if(flag_jishu4 < 350)
      {
        ABCD_judge(D);
        if((abs(Pid_position_x-SetPosition_x)<SetBias+3)&&(abs(Pid_position_y-SetPosition_y)<SetBias+3))
         flag_jishu4++;//小偏差时计时间
       else
         flag_jishu4=0;//否则立刻清零
      }
    }
    
  /*********进入区域七 围绕区域五圆周运动3圈后进入区域九***************/ 
  else if(target_mode == 7)
    {
       static int32 cont_flag = 0;
      extern float Pwm_Px;//电机x
      extern float Pwm_Dx;

      extern float Pwm_Py;//电机y
      extern float Pwm_Dy;
      
      extern float Angle_Px;//电机x
      extern float Angle_Dx;

      extern float Angle_Py;//电机y
      extern float Angle_Dy;
      
      if(cont_flag <100)
      {
        cont_flag ++;
        SetPosition_x = (SetPosition_temp_x4 + SetPosition_temp_x5)/2;
        SetPosition_y = (SetPosition_temp_y4 + SetPosition_temp_y7)/2;
      }
      if((cont_flag < 2000)&&(cont_flag >= 100))//十秒计时
      {
        cont_flag ++;
        Pwm_Px=350;Pwm_Dx=50;Angle_Px=-0.053;Angle_Dx=-0.45;
        Pwm_Py=350;Pwm_Dy=50;Angle_Py=-0.053;Angle_Dy=-0.45;
        SetPosition_x = SetPosition_temp_x5;
        SetPosition_y = SetPosition_temp_y5;
      }
      if((Pid_position_x > SetPosition_temp_x5)&&(Pid_position_y < SetPosition_temp_y5)&&(cont_flag == 2000)&&(goto_nine_flag == 0))
        goto_nine_flag =1;
      if((cont_flag == 2000)&&(goto_nine_flag == 1))//十秒后向区域九走线
      {
        Pwm_Px=400;Pwm_Dx=80;Angle_Px=-0.06;Angle_Dx=-1.3;
        Pwm_Py=400;Pwm_Dy=80;Angle_Py=-0.03;Angle_Dy=-1.2;
        SetPosition_x = SetPosition_temp_x9;//给予第三目标值
        SetPosition_y = SetPosition_temp_y9;
      }
    }
   
  /*********发挥 螺旋进入区域五***************************************/ 
  else if(target_mode == 8)
    {
      static int32 cont_flag = 0;
      extern float Pwm_Px;//电机x
      extern float Pwm_Dx;

      extern float Pwm_Py;//电机y
      extern float Pwm_Dy;
      
      extern float Angle_Px;//电机x
      extern float Angle_Dx;

      extern float Angle_Py;//电机y
      extern float Angle_Dy;
      
      if(cont_flag <100)
      {
        cont_flag ++;
        SetPosition_x= 100;
        SetPosition_y =130;
      }
      else if((cont_flag >=100)&&(cont_flag < 1000))
      {
        cont_flag ++;
        Pwm_Px=350;Pwm_Dx=50;Angle_Px=-0.12;Angle_Dx=-0.8;
        Pwm_Py=350;Pwm_Dy=50;Angle_Py=-0.12;Angle_Dy=-0.8;
        SetPosition_x= SetPosition_temp_x5;
        SetPosition_y =SetPosition_temp_y5;       
      }
      else
      {
        stubbon = 1;
        Pwm_Px=400;Pwm_Dx=80;Angle_Px=-0.054;Angle_Dx=-0.47;
        Pwm_Py=400;Pwm_Dy=80;Angle_Py=-0.054;Angle_Dy=-0.47;
      }
    }

}