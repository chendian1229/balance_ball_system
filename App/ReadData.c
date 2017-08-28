#include "common.h"
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char uartbuf[7]={48,48,48,48,48,48,48};//串口接收缓冲数组 ASCII:48 '0'
int8 i=0;//串口接收计数

void uart4_handler()
{
    extern int32 Pid_position_x;
    extern int32 Pid_position_y;
    
    static int32 x_last=0,y_last=0;
    
  if( uart_query(UART4) == 1 )//接收数据寄存器满
    {
        uart_getchar(UART4, &uartbuf[i]);//处理数据

        if(uartbuf[i] == 0x46)//'F'
        {
            i=-1;

            Pid_position_x = (uartbuf[0]-48)*100+(uartbuf[1]-48)*10+(uartbuf[2]-48);
            Pid_position_y = (uartbuf[3]-48)*100+(uartbuf[4]-48)*10+(uartbuf[5]-48);
            
            //限幅
            if((Pid_position_x>200)||(Pid_position_y>200))
            {
                Pid_position_x=x_last;
                Pid_position_y=y_last;
            
            }
//            if ((x_last!=0)&&(y_last!=0))
//            {
//                //限制突变
//                if((abs(Pid_position_x-x_last>70))||(abs(Pid_position_y-y_last>70)))
//                {
//                    Pid_position_x=x_last;
//                    Pid_position_y=y_last; 
//                }
//            }
            
            x_last=Pid_position_x;
            y_last=Pid_position_y;
            

              allow_control_flag = 1;
            
        }

        i++;
    }
  
}