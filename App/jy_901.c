#include "include.h"

void JY_901_Init()
{
    IIC_Init();
}

void JY_901_GetData(float *roll_angle,float *pitch_angle,float *angular_velocity_z)
{
    uint8 roll_u8_h,roll_u8_l;
    int16 roll_i16;
    uint8 pitch_u8_h,pitch_u8_l;
    int16 pitch_i16;
    uint8 ang_vel_z_u8_h,ang_vel_z_u8_l;
    int16 ang_vel_z_i16;

    //读取三轴角度原始数据
    IIC_Start();
    IIC_SendByte( 0x50<<1 );
    IIC_SendByte( 0x3d );
    IIC_Start();
    IIC_SendByte( (0x50<<1)|0x01 );
    roll_u8_l  = IIC_ReceiveByte_Ack();
    roll_u8_h  = IIC_ReceiveByte_Ack();
    pitch_u8_l = IIC_ReceiveByte_Ack();
    pitch_u8_h = IIC_ReceiveByte_Ack();
    IIC_ReceiveByte_Ack();
    IIC_ReceiveByte_NoAck();
    IIC_Stop();

    //读取三轴角速度原始数据
    IIC_Start();
    IIC_SendByte( 0x50<<1 );
    IIC_SendByte( 0x37 );
    IIC_Start();
    IIC_SendByte( (0x50<<1)|0x01 );
    IIC_ReceiveByte_Ack();
    IIC_ReceiveByte_Ack();
    IIC_ReceiveByte_Ack();
    IIC_ReceiveByte_Ack();
    ang_vel_z_u8_l = IIC_ReceiveByte_Ack();
    ang_vel_z_u8_h = IIC_ReceiveByte_NoAck();
    IIC_Stop();

//    roll_u8_h=0x82;roll_u8_l=0x9c;//test code
//    pitch_u8_h=0xff;pitch_u8_l=0x28;//test code

    //转化为实际角度
    roll_i16  = (roll_u8_h<<8)|roll_u8_l;
    pitch_i16 = (pitch_u8_h<<8)|pitch_u8_l;
    ang_vel_z_i16 = (ang_vel_z_u8_h<<8)|ang_vel_z_u8_l;
    *roll_angle  = roll_i16/32768.0*180.0;
    *pitch_angle = pitch_i16/32768.0*180.0;
    *angular_velocity_z = ang_vel_z_i16/32768.0*2000.0;
}

/******************对陀螺仪输出角度一阶低通滤波*************/
void Angle_filtering(float *roll_date,float *pitch_date)
{
    float K=0.9;                
    static float roll_date_last=0;
    static float pitch_date_last=0;
    
    *roll_date = (float)(K*(*roll_date) + (1-K)*roll_date_last);
    *pitch_date = (float)(K*(*pitch_date) + (1-K)*pitch_date_last);
    
    roll_date_last=*roll_date;
    pitch_date_last=*pitch_date;
    
}