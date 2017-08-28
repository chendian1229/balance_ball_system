#include "include.h"


//中断优先级
void car_init()
{
  set_irq_priority(UART4_RX_TX_IRQn,0);
  set_irq_priority(PIT3_IRQn,1);
  set_irq_priority(PIT0_IRQn,2);
}


