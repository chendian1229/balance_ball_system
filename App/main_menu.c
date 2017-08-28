#include "main_menu.h"
#include "common.h"
#include "include.h"

//函数声明
void PIT0_IRQHandler();  //按键中断
void PIT3_IRQHandler();  
void Analytical_6050_data();
extern void mode_select();

int32 fla1=0,fla2=0,fla3=0;//测试标志
uint8 flag_cot = 0;//启用6050时间计数

//Roll角度
float Roll_Angle;               //测量值
float Target_Roll_Angle = 0.2;        //目标值
//Pitch角度
float Pitch_Angle;              //测量值
float Target_Pitch_Angle = -0.5;       //目标值
//Z轴角速度
float Angular_Velocity_Z;       //测量值          //未用到，暂时未修改
float Target_Angular_Velocity_Z;//目标值

Site_t angle_position_roll= {10,80};    //显示角度坐标 
Site_t angle_position_pitch= {60,80};    //显示角度坐标 

void main_menu_task(void)
{
  
    LCD1_init();
    car_init();
   
    result_e result;
    KEY_MSG_t keymsg;
MAIN_INIT:
    while(get_key_msg(&keymsg) == 1);      //清空按键消息
  
    /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                                   //按键消息初始化
    pit_init_ms(PIT0,5);                                               //pit 定时中断(用于按键定时扫描)
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断复位函数为 PIT0_IRQHandler
    enable_irq(PIT0_IRQn);
    /********串口初始化**********/
    uart_init (UART4, 115200); //串口初始化   uart4配置中断函数，接收openmv传回小球坐标数据
    set_vector_handler(UART4_RX_TX_VECTORn , uart4_handler);
    enable_irq (UART4_RX_TX_IRQn);
    uart_rx_irq_en(UART4);//使能接收中断 
    /********定时器初始化**********/
    pit_init_ms(PIT3, 10); //初始化 PIT3，定时时间为： 10ms
    set_vector_handler(PIT3_VECTORn ,PIT3_IRQHandler);
    enable_irq (PIT3_IRQn);
    

    JY_901_Init();              //6050模块初始化
    JY_901_Init();              //6050模块初始化
    JY_901_Init();              //6050模块初始化
    
    
    
    LCD_clear(BLACK);                     //清屏

    main_menu_init();                       //菜单初始化
    
    
    while(1)
    {
        result = main_menu_deal();
    }
}

/***************************************************************************************************/

result_e camera_stuff_func(void)   
{ 

   reading_flash_from_secotr(); //从扇区中读出数据
  
   Site_t site     = {0, 0};                           //显示图像左上角位置

   KEY_MSG_t keymsg;
   while(get_key_msg(&keymsg) == 1);                   //清空按键消息

   LCD_clear(BCOLOUR);                                 //清屏

   //任务初始化【用户自行补充】
   /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                             //按键消息初始化
    
    while(1)
    { 
       if(write_flag==1)
       {
          reading_flash_from_secotr(); //从扇区中读出数据
           write_flag=0;
       }
      //任务循环执行内容【用户自行补充】
            
       deal_key_event();  //按键程序
       /**************返回菜单*****************/
       if(return_menu1==7)
         if(return_menu2==1)  //返回主菜单
         {
             main_menu_task(); 
             return_menu1=0;
             return_menu2=0;
          }
             
       /***************************************/
       
        LCD_str(angle_position_roll,ftoa(Roll_Angle,1), BLACK,WHITE);      //ftoa(Pitch_Angle,1)
        LCD_str(angle_position_pitch,ftoa(Pitch_Angle,1), BLACK,WHITE); 
        
        DELAY_MS(10);
        
        xy_position();
     }
   
}

/***************************************************************************************************/

result_e STEERING_PID_func(void)             
{
   
   reading_flash_from_secotr(); //从扇区中读出数据
   
  // Site_t site     = {0, 0};                           //显示图像左上角位置
   
   KEY_MSG_t keymsg;
   while(get_key_msg(&keymsg) == 1);                   //清空按键消息

   LCD_clear(BCOLOUR);                                 //清屏


   //任务初始化【用户自行补充】
   /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                             //按键消息初始化
  
    while(1)
    { 
       if(write_flag==1)
       {
          reading_flash_from_secotr(); //从扇区中读出数据
           write_flag=0;
       }
      //任务循环执行内容【用户自行补充】
            
       deal_key_event();  //按键程序
       /**************返回菜单*****************/
       if(return_menu1==7)
         if(return_menu2==1)  //返回主菜单
         {
             main_menu_task(); 
             return_menu1=0;
             return_menu2=0;
          }
             
       /***************************************/
        LCD_str(angle_position_roll,ftoa(Roll_Angle,1), BLACK,WHITE);      //ftoa(Pitch_Angle,1)
        LCD_str(angle_position_pitch,ftoa(Pitch_Angle,1), BLACK,WHITE); 
        
        DELAY_MS(10);
        
        xy_position();
     }
}

/***************************************************************************************************/

result_e MOTOR_PID_func(void)                      
{
   reading_flash_from_secotr(); //从扇区中读出数据
 
   Site_t site     = {0, 0};                           //显示图像左上角位置

   KEY_MSG_t keymsg;
   while(get_key_msg(&keymsg) == 1);                   //清空按键消息

   LCD_clear(BCOLOUR);                                 //清屏
   //LCD_FSTR_CH(site,MOTOR_PID_func_name,FCOLOUR,BCOLOUR);     //显示菜单文字

   //任务初始化【用户自行补充】
   /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                             //按键消息初始化
  
    while(1)
    { 
       if(write_flag==1)
       {
          reading_flash_from_secotr(); //从扇区中读出数据
           write_flag=0;
       }
      //任务循环执行内容【用户自行补充】
            
       deal_key_event();  //按键程序
       /**************返回菜单*****************/
       if(return_menu1==7)
         if(return_menu2==1)  //返回主菜单
         {
             main_menu_task(); 
             return_menu1=0;
             return_menu2=0;
          }
             
       /***************************************/
        LCD_str(angle_position_roll,ftoa(Roll_Angle,1), BLACK,WHITE);      //ftoa(Pitch_Angle,1)
        LCD_str(angle_position_pitch,ftoa(Pitch_Angle,1), BLACK,WHITE); 
        
        DELAY_MS(10);
       
        xy_position();
     }
}
/***************************************************************************************************/

result_e IMG_select_func(void)            
{

   reading_flash_from_secotr(); //从扇区中读出数据
   
   Site_t site     = {0, 0};                           //显示图像左上角位置

   KEY_MSG_t keymsg;
   while(get_key_msg(&keymsg) == 1);                   //清空按键消息

   LCD_clear(BCOLOUR);                                 //清屏
   //LCD_FSTR_CH(site,IMG_select_func_name,FCOLOUR,BCOLOUR);     //显示菜单文字

   //任务初始化【用户自行补充】
   /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                             //按键消息初始化
 
    while(1)
    { 
       if(write_flag==1)
       {
          reading_flash_from_secotr(); //从扇区中读出数据
           write_flag=0;
       }
      //任务循环执行内容【用户自行补充】
            
       deal_key_event();  //按键程序
       /**************返回菜单*****************/
       if(return_menu1==7)
         if(return_menu2==1)  //返回主菜单
         {
             main_menu_task(); 
             return_menu1=0;
             return_menu2=0;
          }
             
       /***************************************/
        LCD_str(angle_position_roll,ftoa(Roll_Angle,1), BLACK,WHITE);      //ftoa(Pitch_Angle,1)
        LCD_str(angle_position_pitch,ftoa(Pitch_Angle,1), BLACK,WHITE); 
        
        DELAY_MS(10);
        
        xy_position();
     }
}
/***************************************************************************************************/

result_e others_func(void)          
{
   
   reading_flash_from_secotr(); //从扇区中读出数据
   
   Site_t site     = {0, 0};                           //显示图像左上角位置


   KEY_MSG_t keymsg;
   while(get_key_msg(&keymsg) == 1);                   //清空按键消息
    
   LCD_clear(BCOLOUR);                                 //清屏
  // LCD_FSTR_CH(site,others_func_name,FCOLOUR,BCOLOUR);     //显示菜单文字

   //任务初始化【用户自行补充】
      /*********************** 按键消息 初始化  ***********************/
    key_event_init();                                             //按键消息初始化
    
    while(1)
    { 
       if(write_flag==1)
       {
          reading_flash_from_secotr(); //从扇区中读出数据
           write_flag=0;
       }
      //任务循环执行内容【用户自行补充】
            
       deal_key_event();  //按键程序
       /**************返回菜单*****************/
       if(return_menu1==7)
         if(return_menu2==1)  //返回主菜单
         {
             main_menu_task(); 
             return_menu1=0;
             return_menu2=0;
          }
             
       /***************************************/
        LCD_str(angle_position_roll,ftoa(Roll_Angle,1), BLACK,WHITE);      //ftoa(Pitch_Angle,1)
        LCD_str(angle_position_pitch,ftoa(Pitch_Angle,1), BLACK,WHITE); 
        
        DELAY_MS(10);
       
        xy_position();
     }
     
}

void Analytical_6050_data()
{
  /****************放在定时器中断里面*******************/
   JY_901_GetData(&Roll_Angle,&Pitch_Angle,&Angular_Velocity_Z);//读取姿态数据
   //解析数据，保留小数点后一位
   Roll_Angle = (int16)(Roll_Angle*10);//翻滚面角度
   Roll_Angle = Roll_Angle/10;
   
   Pitch_Angle = (int16)(Pitch_Angle*10);//倾斜面角度
   Pitch_Angle = Pitch_Angle/10;
   
   //Angle_filtering(&Roll_Angle,&Pitch_Angle);        //两个角度的一阶低通滤波

}

void PIT3_IRQHandler()
{
  extern int32 SetPosition_x;
  extern int32 SetPosition_y;
  extern uint8 target_mode;

    PIT_Flag_Clear(PIT3);       //PIT3标志位清零
    
    mode_select();
    
    Analytical_6050_data();//角度实时监测
    
    if(allow_control_flag == 1)//坐标位置采集成功标志
    {
      if(target_mode != 1)
        Platform_Control();
      flag_cot = 0;
      allow_control_flag=0;
    }//PID控制，清零失败计数
    else if(flag_cot<100)//坐标位置采集失败计数
     flag_cot++;
    
     if(flag_cot == 100)//采集失败时间到1S，停止PID控制，采用6050控制
    {
      stop_Roll_flag = 0;      //采集位置失败，允许MPU650接管电机
      stop_Pitch_flag = 0;
      
      Pid_position_x = 0;
      Pid_position_y = 0;
      allow_control_flag = 0;//采集成功标志          
      Roll_Angle_Control(); //6050控制程序
      Pitch_Angle_Control();
    }
}

void PIT0_IRQHandler()
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT0);
}
