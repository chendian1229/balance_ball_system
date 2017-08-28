#include "common.h"
#include "VCAN_LCD.h"
#include "VCAN_key.h"
#include "include.h"
#include "VCAN_menu.h"


void menu_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab)
{
    menu_display(site,menu,max_tab ,new_tab,max_tab);
}


result_e menu_deal(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab)
{
    KEY_MSG_t keymsg;

    while(get_key_msg(&keymsg) == 1)
    {
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_D:
                {
                    if(*new_tab < max_tab-1)
                    {
                        (*new_tab)++;
                        menu_display(site,menu,*new_tab - 1 ,*new_tab,max_tab);
                    }
                    else
                    {
                        *new_tab=0;
                        menu_display(site,menu,max_tab - 1 ,*new_tab,max_tab);
                    }
                    menu_display(site,menu,*new_tab ,*new_tab,max_tab);
                    break;
                }
            case KEY_U:
                {
                    if(*new_tab == 0)
                    {
                        *new_tab = max_tab - 1;
                        menu_display(site,menu,0 ,*new_tab,max_tab);
                    }
                    else
                    {
                        (*new_tab)--;
                        menu_display(site,menu,*new_tab + 1 ,*new_tab,max_tab);
                    }
                    menu_display(site,menu,*new_tab ,*new_tab,max_tab);
                    break;
                }
            case KEY_A:
                {
                    return RESULT_INIT;
                }
            case KEY_B:
                {
                    menu_select=*new_tab;            //记录选择的菜单的哪个选项  
                    return menu[*new_tab].pfunc();// menu[*new_tab] *new_tab=0时对应菜单第一个选项 即摄像头参数  
                                                                   //*new_tab=1时对应菜单第二个选项 即舵机PID
                                                                   //*new_tab=2时对应菜单第三个选项 即电机PID
                                                                   //*new_tab=3时对应菜单第四个选项 即LCD图像选择
                                                                   //*new_tab=4时对应菜单第五个选项 即其他 
                }
                
            default:
                break;

            }

        }
    }

    return RESULT_NULL;
}

void menu_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab)
{
/*    //定义背景延时
#define SELECT_BG         YELLOW   //当前选中，而且没有改变
#define NO_SELECT_BG      BCOLOUR     //没有选中，而且没有改变（普通的就是这样）

    //定义文字颜色
#define SELECT_FC            BLUE    //当前选中，而且没有改变
#define NO_SELECT_FC         FCOLOUR   //没有选中，而且改变了
*/
 
#define SELECT_BG         YELLOW   //当前选中，而且没有改变
#define NO_SELECT_BG      BLACK     //没有选中，而且没有改变（普通的就是这样）

    //定义文字颜色
#define SELECT_FC            BLUE    //当前选中，而且没有改变
#define NO_SELECT_FC         WHITE   //没有选中，而且改变了
  
    uint8  i = 0;
    uint16 bkColor;
    uint16 Color;
    Site_t  sitetmp;
    sitetmp.x =  site.x;

    if(tab == max_tab)      //显示全部
    {
        i = max_tab - 1;    //循环的次数
        tab = 0;
    }

    do
    {
        if(tab == new_tab)
        {

            Color   =  SELECT_FC;
            bkColor =  SELECT_BG;

        }
        else
        {
            Color   =  NO_SELECT_FC;
            bkColor =  NO_SELECT_BG;
        }


        //显示文字
        //LCD_num_C(VAR_SITE(tab), VAR_VALUE(tab), Color, bkColor);
        sitetmp.y = site.y +  tab * LCD_CH_H ;
        LCD_FSTR_CH_FUNC(sitetmp,menu[tab].strbuff,menu[tab].strnum,Color,bkColor);
        tab++;
    }
    while(i--);         //tab != VAR_MAX 的时候，执行一次就跳出
}