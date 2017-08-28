#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
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
#include  "VCAN_MMA7455.h"      //������ٶ�MMA7455
#include  "VCAN_NRF24L0.h"      //����ģ��NRF24L01+
#include  "VCAN_RTC_count.h"    //RTC ʱ��ת��
#include  "VCAN_camera.h"       //����ͷ��ͷ�ļ�
#include  "VCAN_LCD.h"          //Һ����ͷ�ļ�
#include  "ff.h"                //FatFs
#include  "VCAN_TSL1401.h"      //����CCD
#include  "VCAN_key_event.h"    //������Ϣ����
#include  "VCAN_NRF24L0_MSG.h"  //����ģ����Ϣ����
#include  "VCAN_BMP.h"          //BMP
#include  "vcan_img2sd.h"       //�洢ͼ��sd��һ���ļ�
#include  "vcan_sd_app.h"       //SD��Ӧ�ã���ʾsd����ͼƬ�̼���
#include  "Vcan_touch.h"        //��������
#include  "VCAN_computer.h"     //�๦�ܵ�������



#include  "LCD.h"
#include  "DLib_Product_string.h"
#include  "car.h"   




#include  "control.h"

#include  "ReadData.h"

#include  "menu.h"

#include  "6050.h"
/**********************�����ļ�*****************/
#include "iic.h"                //���ģ��IICͨѶ
#include "jy_901.h"             //IIC����6050�����Ƕȣ�����ģ�飩
#include "ftoa.h"               //�ַ���ת����
#include "pid.h" 

   extern float Roll_Angle;               //����ֵ
   extern float Target_Roll_Angle;        //Ŀ��ֵ
//Pitch�Ƕ�
   extern float Pitch_Angle;              //����ֵ
   extern float Target_Pitch_Angle;       //Ŀ��ֵ
//Z����ٶ�
   extern float Angular_Velocity_Z;       //����ֵ          //δ�õ�����ʱδ�޸�
   extern float Target_Angular_Velocity_Z;//Ŀ��ֵ

   #include "VCAN_menu.h"  //  �˵�
   
//����
#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //������������������ȷ����ȫ
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


extern uint8 menu_select; //��¼ѡ��Ĳ˵����ĸ�ѡ��

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

extern uint8 memory41;  //��¼LCDͼ��ѡ��
extern uint8 memory42;
extern uint16 memory43;
extern uint16 memory44;
extern uint32 memory45;
extern uint32 memory46;
extern uint32 memory49;

extern uint8 memory51;  //��¼����
extern uint8 memory52;  //��¼����
extern uint16 memory53;  //��¼����
extern uint16 memory54;
extern uint32 memory55;
extern uint32 memory56;
extern uint32 memory59;

extern uint8 write_flag;






#endif  //__INCLUDE_H__
