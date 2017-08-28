#include "common.h"
#include "include.h"
uint32 mode_number = 0;
int mode;

void xy_position()
{

    Site_t site = {10,10};
    Size_t size = {60,10};
  
    LCD_char(site,'x', BLACK,WHITE);
    site.x = 40;   
    site.y = 10;   
    LCD_rectangle(site, size, WHITE);
    LCD_num(site,Pid_position_x, BLACK,WHITE);
    site.x = 10;   
    site.y = 30;
    LCD_char(site,'y', BLACK,WHITE);
    site.x = 40;   
    site.y = 30;
    size.W = 60;
    size.H = 30;  // W = 50 ,H = 60
    LCD_rectangle(site, size, WHITE);
    LCD_num(site,Pid_position_y, BLACK,WHITE);
}

void menu()
{
  
}