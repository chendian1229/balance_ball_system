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
                    menu_select=*new_tab;            //��¼ѡ��Ĳ˵����ĸ�ѡ��  
                    return menu[*new_tab].pfunc();// menu[*new_tab] *new_tab=0ʱ��Ӧ�˵���һ��ѡ�� ������ͷ����  
                                                                   //*new_tab=1ʱ��Ӧ�˵��ڶ���ѡ�� �����PID
                                                                   //*new_tab=2ʱ��Ӧ�˵�������ѡ�� �����PID
                                                                   //*new_tab=3ʱ��Ӧ�˵����ĸ�ѡ�� ��LCDͼ��ѡ��
                                                                   //*new_tab=4ʱ��Ӧ�˵������ѡ�� ������ 
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
/*    //���屳����ʱ
#define SELECT_BG         YELLOW   //��ǰѡ�У�����û�иı�
#define NO_SELECT_BG      BCOLOUR     //û��ѡ�У�����û�иı䣨��ͨ�ľ���������

    //����������ɫ
#define SELECT_FC            BLUE    //��ǰѡ�У�����û�иı�
#define NO_SELECT_FC         FCOLOUR   //û��ѡ�У����Ҹı���
*/
 
#define SELECT_BG         YELLOW   //��ǰѡ�У�����û�иı�
#define NO_SELECT_BG      BLACK     //û��ѡ�У�����û�иı䣨��ͨ�ľ���������

    //����������ɫ
#define SELECT_FC            BLUE    //��ǰѡ�У�����û�иı�
#define NO_SELECT_FC         WHITE   //û��ѡ�У����Ҹı���
  
    uint8  i = 0;
    uint16 bkColor;
    uint16 Color;
    Site_t  sitetmp;
    sitetmp.x =  site.x;

    if(tab == max_tab)      //��ʾȫ��
    {
        i = max_tab - 1;    //ѭ���Ĵ���
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


        //��ʾ����
        //LCD_num_C(VAR_SITE(tab), VAR_VALUE(tab), Color, bkColor);
        sitetmp.y = site.y +  tab * LCD_CH_H ;
        LCD_FSTR_CH_FUNC(sitetmp,menu[tab].strbuff,menu[tab].strnum,Color,bkColor);
        tab++;
    }
    while(i--);         //tab != VAR_MAX ��ʱ��ִ��һ�ξ�����
}