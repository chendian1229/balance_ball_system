#ifndef _VCAN_MENU_H_
#define _VCAN_MENU_H_


typedef  enum
{
    RESULT_NULL,
    RESULT_INIT,            //���³�ʼ��
    RESULT_TO_MENU,         //��һ�� �˵�
}result_e;

typedef  result_e  ap_func(void);

typedef struct
{
    void    *   strbuff;      //�ַ���ͼ��
    uint32      strnum;       //�����ַ���Ŀ
    ap_func *   pfunc;        //ִ�к���

}menu_t;

#define VCAN_MENU(s,fc)        {(void *)(s),sizeof(s)/LCD_CH_SIZE,fc}         //�˵�����һ�������� �����ֿ� ���ڶ����� ִ�к���


#define LCD_FSTR_CH_FUNC     LCD_FStr_CH

extern void        menu_init(Site_t site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);
extern result_e    menu_deal(Site_t site,menu_t *menu,uint8 tab,uint8 *new_tab,uint8 max_tab);
extern void        menu_display(Site_t  site,menu_t *menu,uint8 tab,uint8 new_tab,uint8 max_tab);


//extern uint8 menu_select; //��¼ѡ��Ĳ˵����ĸ�ѡ��


#endif  //_VCAN_MENU_H_