#ifndef __IIC_H__
#define __IIC_H__


void  IIC_Init();
void  IIC_Start();
uint8 IIC_SendByte(uint8 byte);
uint8 IIC_ReceiveByte_Ack();
uint8 IIC_ReceiveByte_NoAck();
void  IIC_Stop();

/*
 * 使用范例：   读取数据（单字节或连续）
 *              IIC_Start();
 *              IIC_SendByte( 0x50<<1 );
 *              IIC_SendByte( 0x3d );
 *              IIC_Start();
 *              IIC_SendByte( (0x50<<1)|0x01 );
 *              byte(1)   = IIC_ReceiveByte_Ack();
 *              byte(2)   = IIC_ReceiveByte_Ack();
 *                            ...
 *              byte(n-1) = IIC_ReceiveByte_Ack();
 *              byte(n)   = IIC_ReceiveByte_NoAck();
 *              IIC_Stop();
 *
 *              写入数据（单字节或连续）
 *              IIC_Start();
 *              IIC_SendByte( 0x50<<1 );
 *              IIC_SendByte( 0x3d );
 *              IIC_SendByte( 0xxx );//byte(0)
 *              IIC_SendByte( 0xxx );//byte(1)
 *                         ...
 *              IIC_SendByte( 0xxx );//byte(n)
 *              IIC_Stop();
 */


#endif