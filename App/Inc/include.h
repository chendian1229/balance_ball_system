#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC
#include  "MK60_usb.h"      //usb

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_MMA7455.h"      //三轴加速度MMA7455
#include  "VCAN_NRF24L0.h"      //无线模块NRF24L01+
#include  "VCAN_RTC_count.h"    //RTC 时间转换
#include  "VCAN_camera.h"       //摄像头总头文件
#include  "VCAN_LCD.h"          //液晶总头文件
#include  "ff.h"                //FatFs
#include  "VCAN_TSL1401.h"      //线性CCD
#include  "VCAN_key_event.h"    //按键消息处理
#include  "VCAN_NRF24L0_MSG.h"  //无线模块消息处理
#include  "VCAN_BMP.h"          //BMP
#include  "vcan_img2sd.h"       //存储图像到sd卡一个文件
#include  "vcan_sd_app.h"       //SD卡应用（显示sd看上图片固件）
#include  "Vcan_touch.h"        //触摸驱动
#include  "VCAN_computer.h"     //多功能调试助手



#include  "LCD.h"
#include  "DLib_Product_string.h"
#include  "car.h"   




#include  "control.h"

#include  "ReadData.h"

#include  "menu.h"

#include  "6050.h"
/**********************自拟文件*****************/
#include "iic.h"                //软件模拟IIC通讯
#include "jy_901.h"             //IIC访问6050解析角度（串口模块）
#include "ftoa.h"               //字符串转化库
#include "pid.h" 

   extern float Roll_Angle;               //测量值
   extern float Target_Roll_Angle;        //目标值
//Pitch角度
   extern float Pitch_Angle;              //测量值
   extern float Target_Pitch_Angle;       //目标值
//Z轴角速度
   extern float Angular_Velocity_Z;       //测量值          //未用到，暂时未修改
   extern float Target_Angular_Velocity_Z;//目标值

   #include "VCAN_menu.h"  //  菜单
   
//扇区
#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全
#define SECTOR_NUM1 (FLASH_SECTOR_NUM-4)

extern uint8  var1; 
extern uint8  var2;
extern uint16 var3; 
extern uint16 var4;
extern uint32 var5;
extern uint32 var6;
extern uint32 var7;
extern uint32 var8;
extern uint32 var9;


extern uint8 menu_select; //记录选择的菜单的哪个选项

extern uint8 return_menu1;
extern uint8 return_menu2;

extern uint8 fw_confirm1;
extern uint8 fw_confirm2;

extern uint8 memoryflag1;

extern uint8 memory_flag1;
extern uint8 memory_flag2;
extern uint8 memory_flag3;
extern uint8 memory_flag4;
extern uint8 memory_flag5;
extern uint8 memory_flag6;
extern uint8 memory_flag9;

extern uint8 memory11;  
extern uint8 memory12;  
extern uint16 memory13;
extern uint16 memory14;
extern uint32 memory15;
extern uint32 memory16;
extern uint32 memory19;

extern uint8 memory21;  
extern uint8 memory22;  
extern uint16 memory23;
extern uint16 memory24;
extern uint32 memory25;
extern uint32 memory26;
extern uint32 memory29;

extern uint8 memory31;  
extern uint8 memory32;  
extern uint16 memory33;
extern uint16 memory34;
extern uint32 memory35;
extern uint32 memory36;
extern uint32 memory39;

extern uint8 memory41;  //记录LCD图像选择
extern uint8 memory42;
extern uint16 memory43;
extern uint16 memory44;
extern uint32 memory45;
extern uint32 memory46;
extern uint32 memory49;

extern uint8 memory51;  //记录其他
extern uint8 memory52;  //记录其他
extern uint16 memory53;  //记录其他
extern uint16 memory54;
extern uint32 memory55;
extern uint32 memory56;
extern uint32 memory59;

extern uint8 write_flag;






#endif  //__INCLUDE_H__
