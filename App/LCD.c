#include "include.h"
#include "common.h"

Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
Size_t size;                   //��ʾ����ͼ���С

void LCD1_init();

void LCD1_init()
{
    LCD_init();
  
//    size.H =128;
//    size.W =128;
    
    size.H =100;
    size.W =100;

}

