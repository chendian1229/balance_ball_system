#include "include.h"
#include "common.h"

Site_t site     = {0, 0};                           //显示图像左上角位置
Size_t size;                   //显示区域图像大小

void LCD1_init();

void LCD1_init()
{
    LCD_init();
  
//    size.H =128;
//    size.W =128;
    
    size.H =100;
    size.W =100;

}

