#include "common.h"
#include "VCAN_LCD.h"
#include "VCAN_UI_VAR.h"
#include "VCAN_NRF24L0_MSG.h"
#include "include.h"

#define VAR_VALUE(var_tab)      num_info[var_tab].val       //ָ����ŵı�����ֵ
#define VAR_OLDVALUE(var_tab)   num_info[var_tab].oldval    //ָ����ŵı��������ȷ��ֵ
#define VAR_MAXVALUE(var_tab)   num_info[var_tab].maxval
#define VAR_MINVALUE(var_tab)   num_info[var_tab].minval
#define VAR_SITE(var_tab)       num_info[var_tab].site

static uint8  car_ctrl = CAR_NULL;
extern uint8  var1, var2;
extern uint16  var3,var4;
extern uint32 var5, var6, var7,var8,var9;

uint32 *var_addr[VAR_MAX] = {(uint32 *)&car_ctrl,(uint32 *)&var1, (uint32 *)&var2, (uint32 *)&var3, (uint32 *)&var4, (uint32 *)&var5, (uint32 *)&var6, (uint32 *)&var7, (uint32 *)&var8, (uint32 *)&var9};

//VAR_MAXΪ������Ŀ
ui_var_info_t  num_info[VAR_MAX] =
{

    //val,oldval,���ڵ���key_event_init��ʱ������Ӧ�����︳ֵ���������������ֵ��������д
    //��Ҫ����minval,maxval,{x,y}
    //���ע����Сֵ��Ҫ�������ֵ
  
//  {val,oldval,minval,maxval,{x,y}}
    {0, 0, 0, CAR_CTRL_MAX, {103,102}},      //���� car_ctrl��
    {0, 0, 0, 200, {103, 0}},                //���� var1��
    {0, 0, 0, 200, {103, 17}},               //���� var2��
    {0, 0, 0, 800, {103, 34}},               //���� var3��
    {0, 0, 0, 1000, {103, 51}},               //���� var4��
    {0, 0, 0, 500, {103, 68}},             //���� var5��
    {0, 0, 0, 500, {103, 85}},              //���� var6��
    {0, 0, 0, 500, {0, 102}},               //���� var7��
    {0, 0, 0, 500, {50, 102}},               //���� var8��
    {0, 0, 0, 500, {103, 102}}               //���� var8��
};

uint8   new_tab = 0;        //��ǰѡ��ı������
uint32  last_tab;           //�����յ��ı������



//ͬ��ָ����ֵ��tab Ϊ VAR_NUM ʱ��ʾȫ��ͬ����С����ͬ����Ӧ��
//������ͬ������ʾȫ������Ϊ�п���ͬ��ʧ�ܡ�
//static uint8 var_syn(uint8 tab);         //ͬ�����ݣ�1��ʾ�ɹ���0��ʾʧ��

void save_var2buff(var_tab_e var_num, uint8 *sendbuf);              //����Ҫ���͵ı���������д�뵽������

void var_init()
{
    uint8   var_num;
    uint32  vartemp;

    for(var_num = 0; var_num < VAR_MAX; var_num++)
    {
        get_var((var_tab_e)var_num, &vartemp);
        num_info[var_num].val       = vartemp;
        num_info[var_num].oldval    = vartemp;

        //�����Сֵ�����ֵ
        ASSERT(num_info[var_num].maxval  >=  num_info[var_num].minval );
    }
}

void save_var(var_tab_e var_tal, uint32 var_data)
{
    if(var_tal <= VAR_8BIT)
    {
        *((uint8 *)(var_addr[var_tal])) = var_data;
    }
    else if(var_tal <= VAR_16BIT)
    {
        *((uint16 *)(var_addr[var_tal])) = var_data;
    }
    else if(var_tal <= VAR_32BIT)
    {
        *((uint32 *)(var_addr[var_tal])) = var_data;
    }

    VAR_VALUE(var_tal) = var_data;
    VAR_OLDVALUE(var_tal) = var_data;
}

void get_var(var_tab_e var_tal, uint32 *var_data)
{
    if(var_tal <= VAR_8BIT)
    {
        *var_data = (uint32) * ((uint8 *)(var_addr[var_tal]));
    }
    else if(var_tal <= VAR_16BIT)
    {
        *var_data = (uint32) * ((uint16 *)(var_addr[var_tal])) ;
    }
    else if(var_tal <= VAR_32BIT)
    {
        *var_data = (uint32) * ((uint32 *)(var_addr[var_tal]));
    }
}


void updata_var(var_tab_e var_tal)
{
    uint32 vartemp;

    get_var(var_tal, &vartemp);

    VAR_VALUE(var_tal) = vartemp;
}
 
//�Ա����ļӼ����д���  VAR_ADD
void var_value(ui_var_event_e ctrl)
{
    ASSERT(new_tab < VAR_MAX);


    //�޸ĵ�ǰ������ֵ
    switch(ctrl)    //�ж�����ֵ
    {
      case VAR_ADD:       //����ֵΪ ��1  
        if(VAR_VALUE(new_tab) < VAR_MAXVALUE(new_tab)) //���ڵ�ֵС�����ֵ��1
        {
            VAR_VALUE(new_tab)++;
        }
        else              //���������Сֵ
        {
            VAR_VALUE(new_tab) = VAR_MINVALUE(new_tab);       //���ֵ��1Ϊ��Сֵ
        }
        break;

    case VAR_SUB:       //����ֵΪ ��1  
        if(VAR_VALUE(new_tab) > VAR_MINVALUE(new_tab))   //���ڵ�ֵ������Сֵ��1
        {
            VAR_VALUE(new_tab)--;
        }
        else             //����������ֵ
        {
            VAR_VALUE(new_tab) = VAR_MAXVALUE(new_tab) ;//��Сֵ��һΪ���ֵ
        }
        break;

    case VAR_ADD_HOLD:
        if(   (VAR_MAXVALUE(new_tab) - VAR_VALUE(new_tab))  >  VAR_VALUE_HOLE_OFFSET )
        {
            VAR_VALUE(new_tab) += VAR_VALUE_HOLE_OFFSET;
        }
        else
        {
            VAR_VALUE(new_tab) = VAR_MINVALUE(new_tab);
        }
        break;

    case VAR_SUB_HOLD:
        if( ( VAR_VALUE(new_tab) - VAR_MINVALUE(new_tab)) > VAR_VALUE_HOLE_OFFSET  )
        {
            VAR_VALUE(new_tab) -= VAR_VALUE_HOLE_OFFSET;
        }
        else
        {
            VAR_VALUE(new_tab) = VAR_MAXVALUE(new_tab) ;//��Сֵ��һΪ���ֵ
        }
        break;

    default:                        //��Чѡ�񣬲���Ҫ�л�
        break;
    }
    
    return_menu1=new_tab;
    fw_confirm1=new_tab;
    
    memoryflag1=new_tab;

    var_display(new_tab);
}

//�Ա�������ѡ��
void var_select(ui_var_event_e  ctrl)
{
    ASSERT(new_tab < VAR_MAX);

    uint8 old_tab = new_tab;       //�ȱ��ݵ�ǰ�������

    //�л�����һ������
    switch(ctrl)
    {
    case VAR_NEXT:                      //��һ��
        new_tab++;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = 0;               //��ͷ��ʼ
        }
        break;

    case VAR_PREV:                      //��һ��
        new_tab--;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = VAR_MAX - 1;     //��β��ʼ
        }
        break;

    case VAR_NEXT_HOLD:                 //����
        new_tab += VAR_SELECT_HOLD_OFFSET;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = 0;               //��ͷ��ʼ
        }
        break;

    case VAR_PREV_HOLD:                 //����
        new_tab -= VAR_SELECT_HOLD_OFFSET;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = VAR_MAX - 1;     //��β��ʼ
        }
        break;

    default:                        //��Чѡ�񣬲���Ҫ�л�
        return;
    }
    
    
    var_display(old_tab);               //������һ������

    var_display(new_tab);              //����ǰ������

}


//ȷ�ϵ�ǰѡ���
void var_ok()
{
    ASSERT(new_tab < VAR_MAX);

    //�Ƚ��Ƿ��иı�ֵ
    if(VAR_VALUE(new_tab) != VAR_OLDVALUE(new_tab))   //ֵ�ı��ˣ�����Ҫ����
    {
        var_syn(new_tab);          //ͬ���µ�ֵ
    }

    var_display(new_tab);
}

//ȡ����ǰѡ���ֵ  OK
void val_cancel()
{
    ASSERT(new_tab < VAR_MAX);

    //ֱ�ӻ�ԭ��ǰֵ
    VAR_VALUE(new_tab) = VAR_OLDVALUE(new_tab);

    var_display(new_tab);
}



//��ʾָ����ֵ��tab Ϊ VAR_MAX ʱ��ʾȫ����ʾ��С������ʾ��Ӧ��

void var_display(uint8 tab)
{
#if UI_VAR_USE_LCD
/*
    //���屳����ʱ
#define SELECT_NO_CHANGE_BG         WHITE   //��ǰѡ�У�����û�иı�
#define SELECT_CHANGE_BG            WHITE   //��ǰѡ�У����Ҹı���
#define NO_SELECT_NO_CHANGE_BG      RED     //û��ѡ�У�����û�иı䣨��ͨ�ľ���������
#define NO_SELECT_CHANGE_BG         RED     //û��ѡ�У����Ҹı���

    //����������ɫ
#define SELECT_NO_CHANGE            BLUE    //��ǰѡ�У�����û�иı�
#define SELECT_CHANGE               GREEN   //��ǰѡ�У����Ҹı���
#define NO_SELECT_NO_CHANGE         BLUE    //û��ѡ�У�����û�иı䣨��ͨ�ľ���������
#define NO_SELECT_CHANGE            GREEN   //û��ѡ�У����Ҹı���
*/
    //���屳����ʱ
#define SELECT_NO_CHANGE_BG         YELLOW   //��ǰѡ�У�����û�иı�
#define SELECT_CHANGE_BG            YELLOW   //��ǰѡ�У����Ҹı���
#define NO_SELECT_NO_CHANGE_BG      BLACK     //û��ѡ�У�����û�иı䣨��ͨ�ľ���������
#define NO_SELECT_CHANGE_BG         BLACK     //û��ѡ�У����Ҹı���

    //����������ɫ
#define SELECT_NO_CHANGE            WHITE    //��ǰѡ�У�����û�иı�
#define SELECT_CHANGE               GREEN   //��ǰѡ�У����Ҹı���
#define NO_SELECT_NO_CHANGE         WHITE    //û��ѡ�У�����û�иı䣨��ͨ�ľ���������
#define NO_SELECT_CHANGE            GREEN   //û��ѡ�У����Ҹı���  
  
  
    uint8  i = 0;
    uint16 bkColor;
    uint16 Color;

    ASSERT((new_tab < VAR_MAX) && (tab <= VAR_MAX));

    if(tab == VAR_MAX)      //��ʾȫ��
    {
        i = VAR_MAX - 1;    //ѭ���Ĵ���
        tab = 0;
    }

    do
    {
        if(tab == new_tab)
        {
            //��ʾ��ǰ��ֵ���ж�ֵ�Ƿ�ı�
            if(VAR_VALUE(tab) == VAR_OLDVALUE(tab)) //ֵû�ı䣬����Ҫ����
            {
               Color   =  SELECT_NO_CHANGE;
                bkColor =  SELECT_NO_CHANGE_BG;
            }
            else
            {
                Color   =  SELECT_CHANGE;
                bkColor =  SELECT_CHANGE_BG;
            }
        }
        else
        {
            //��ʾ�ǵ�ǰ��ֵ
            if(VAR_VALUE(tab) == VAR_OLDVALUE(tab)) //ֵû�ı䣬����Ҫ����
            {
                Color   =  NO_SELECT_NO_CHANGE;
                bkColor =  NO_SELECT_NO_CHANGE_BG;

            }
            else
            {
                Color   =  NO_SELECT_CHANGE;
                bkColor =  NO_SELECT_CHANGE_BG;
            }
        }

        //��ʾ����
        LCD_num_C(VAR_SITE(tab), VAR_VALUE(tab), Color, bkColor);

        tab++;
    }
    while(i--);         //tab != VAR_MAX ��ʱ��ִ��һ�ξ�����
#else
    tab = tab;          //�������뾯��
#endif
}

void save_var2buff(var_tab_e var_num, uint8 *sendbuf)
{
    uint32 temp;
    get_var(var_num, &temp);
    *((uint32 *)&sendbuf[COM_LEN]) = (uint32)var_num;
    *((uint32 *)&sendbuf[COM_LEN + sizeof(uint32)]) = temp;
}

//ͬ��ָ����ֵ��tab Ϊ VAR_MAX ʱ��ʾȫ��ͬ����С����ͬ����Ӧ��
uint8 var_syn(uint8 tab)
{
    ASSERT((new_tab < VAR_MAX) && (tab <= VAR_MAX));

    uint8  i = 0;
    uint8 tempbuff[DATA_PACKET];
    uint32 oldvalue;

    if(tab == VAR_MAX)
    {
        i = VAR_MAX - 1;
        tab = 0;
    }

    do
    {
        oldvalue = VAR_OLDVALUE(tab);                   //���ݾɵ�ֵ

        //��ֵ���Ƶ���Ӧ�ı���
        save_var((var_tab_e)tab, VAR_VALUE(tab));
        
        fw_confirm2=VAR_VALUE(tab);
        return_menu2=VAR_VALUE(tab);
       
        if(menu_select==0)      //�ж�ѡ��ĵڼ����˵�
        {   
          
          if(memoryflag1==1)   //�ж�ѡ��ĵڼ�������
          {     
            memory11=VAR_VALUE(tab);  //��¼���θı��ֵ 
            
            memory_flag1=1;
          } 
          else if(memoryflag1==2)
          {
            memory12=VAR_VALUE(tab);

            memory_flag2=1;
            //LCD_off_flag=1;
          }
          else if(memoryflag1==3)
          {
            memory13=VAR_VALUE(tab);

            memory_flag3=1;
          }
          else if(memoryflag1==4)
          {
            memory14=VAR_VALUE(tab);

            memory_flag4=1;
          }
          else if(memoryflag1==5)
          {
            memory15=VAR_VALUE(tab);

            memory_flag5=1;
          }          
          else if(memoryflag1==6)
          {
            memory16=VAR_VALUE(tab);
            
            memory_flag6=1;
          }
          else if(memoryflag1==9)
          {
             memory19=VAR_VALUE(tab);
            
             memory_flag9=1;
          }
          else if(memoryflag1==8)     //��ֹʲô�����ı� �Ͱ�ȷ�� �����Ϳ���ʹÿ���˵������ݶ��ܱ�������
          {
             if(memory_flag1==1)
                ;
             else
               memory11=flash_read(SECTOR_NUM, 4, uint8);
             if(memory_flag2==1)
               ;
             else 
               memory12=flash_read(SECTOR_NUM, 8, uint8);
             if(memory_flag3==1)
                ;
             else
               memory13=flash_read(SECTOR_NUM, 12, uint16);
             if(memory_flag4==1)
               ;
             else      
               memory14=flash_read(SECTOR_NUM, 20, uint16);
             if(memory_flag5==1)
               ;
             else 
               memory15=flash_read(SECTOR_NUM, 28, uint32);  
             if(memory_flag6==1)
               ;
             else   
               memory16=flash_read(SECTOR_NUM, 44, uint32);
             if(memory_flag9==1)
               ;
             else
               memory19=flash_read(SECTOR_NUM1,32, uint32);
            if(menu_select!=1)
            {
              memory21=flash_read(SECTOR_NUM, 60, uint8);
              memory22=flash_read(SECTOR_NUM, 64, uint8);
              memory23=flash_read(SECTOR_NUM, 68, uint16);
              memory24=flash_read(SECTOR_NUM, 76, uint16);
              memory25=flash_read(SECTOR_NUM, 84, uint32);
              memory26=flash_read(SECTOR_NUM, 100, uint32);
              memory29=flash_read(SECTOR_NUM1, 48, uint32);
            }
            if(menu_select!=2)
            {
              memory31=flash_read(SECTOR_NUM, 116, uint8);
              memory32=flash_read(SECTOR_NUM, 120, uint8);
              memory33=flash_read(SECTOR_NUM, 124, uint16);
              memory34=flash_read(SECTOR_NUM, 132, uint16);
              memory35=flash_read(SECTOR_NUM, 140, uint32);
              memory36=flash_read(SECTOR_NUM, 156, uint32);
              memory39=flash_read(SECTOR_NUM1, 64, uint32);
            }
            if(menu_select!=3)
            {
              memory41=flash_read(SECTOR_NUM, 172, uint8);
              memory42=flash_read(SECTOR_NUM, 176, uint8);
              memory43=flash_read(SECTOR_NUM, 180, uint16);
              memory44=flash_read(SECTOR_NUM, 188, uint32);
              memory45=flash_read(SECTOR_NUM, 196, uint32);
              memory46=flash_read(SECTOR_NUM, 212, uint32);
              memory49=flash_read(SECTOR_NUM1, 80, uint32);
            }
            if(menu_select!=4)
            {
              memory51=flash_read(SECTOR_NUM, 228, uint8);
              memory52=flash_read(SECTOR_NUM, 232, uint16);
              memory53=flash_read(SECTOR_NUM, 236, uint16);
              memory54=flash_read(SECTOR_NUM, 244, uint32);
              memory55=flash_read(SECTOR_NUM1, 0, uint32);    
              memory56=flash_read(SECTOR_NUM1, 16, uint32); 
              memory59=flash_read(SECTOR_NUM1, 96, uint32);
            }
          } 
          memoryflag1=0;

        }
        else if(menu_select==1)   //�ж�ѡ��ĵڼ����˵�
        {
          if(memoryflag1==1)       //�ж�ѡ��ĵڼ�������
          {
            memory21=VAR_VALUE(tab);  //��¼���θı��ֵ 
           /* memory22=flash_read(SECTOR_NUM, 64, uint8);   //δ�ı��ֱֵ�Ӵ�flash�ж��� ��ֹ�����˵��е�����д�����
            memory23=flash_read(SECTOR_NUM, 68, uint16);
            memory24=flash_read(SECTOR_NUM, 76, uint16);
            memory25=flash_read(SECTOR_NUM, 84, uint32);
            memory26=flash_read(SECTOR_NUM, 100, uint32);*/
            memory_flag1=1;
          }
          else if(memoryflag1==2)
          {
            //memory21=flash_read(SECTOR_NUM, 60, uint8);
            memory22=VAR_VALUE(tab);
           /* memory23=flash_read(SECTOR_NUM, 68, uint16);
            memory24=flash_read(SECTOR_NUM, 76, uint16);
            memory25=flash_read(SECTOR_NUM, 84, uint32);
            memory26=flash_read(SECTOR_NUM, 100, uint32);*/
             memory_flag2=1;
          }
          else if(memoryflag1==3)
          {        
            /*memory21=flash_read(SECTOR_NUM, 60, uint8);
            memory22=flash_read(SECTOR_NUM, 64, uint8);*/
            memory23=VAR_VALUE(tab);
           /* memory24=flash_read(SECTOR_NUM, 76, uint16);
            memory25=flash_read(SECTOR_NUM, 84, uint32);
            memory26=flash_read(SECTOR_NUM, 100, uint32);*/
            memory_flag3=1;
          }
          else if(memoryflag1==4)
          {
            /* memory21=flash_read(SECTOR_NUM, 60, uint8);
             memory22=flash_read(SECTOR_NUM, 64, uint8);
             memory23=flash_read(SECTOR_NUM, 68, uint16);  */           
             memory24=VAR_VALUE(tab);
            /* memory25=flash_read(SECTOR_NUM, 84, uint32);
             memory26=flash_read(SECTOR_NUM, 100, uint32);*/
             memory_flag4=1;
          }
          else if(memoryflag1==5)
          {
             /*memory21=flash_read(SECTOR_NUM, 60, uint8);
             memory22=flash_read(SECTOR_NUM, 64, uint8);
             memory23=flash_read(SECTOR_NUM, 68, uint16);
             memory24=flash_read(SECTOR_NUM, 76, uint16);*/
             memory25=VAR_VALUE(tab);
            // memory26=flash_read(SECTOR_NUM, 100, uint32);
             memory_flag5=1;
          }
          else if(memoryflag1==6)
          {
             /*memory21=flash_read(SECTOR_NUM, 60, uint8);
             memory22=flash_read(SECTOR_NUM, 64, uint8);
             memory23=flash_read(SECTOR_NUM, 68, uint16);
             memory24=flash_read(SECTOR_NUM, 76, uint16);
             memory25=flash_read(SECTOR_NUM, 84, uint32);*/
             memory26=VAR_VALUE(tab);
             memory_flag6=1;
          }
          else if(memoryflag1==9)
          {
             memory29=VAR_VALUE(tab);
             memory_flag9=1;
          }
          else if(memoryflag1==8)     //��ֹʲô�����ı� �Ͱ�ȷ�� �����Ϳ���ʹÿ���˵������ݶ��ܱ�������
          {
            if(menu_select!=0)
            {
              memory11=flash_read(SECTOR_NUM, 4, uint8);
              memory12=flash_read(SECTOR_NUM, 8, uint8);
              memory13=flash_read(SECTOR_NUM, 12, uint16);
              memory14=flash_read(SECTOR_NUM, 20, uint16);
              memory15=flash_read(SECTOR_NUM, 28, uint32);
              memory16=flash_read(SECTOR_NUM, 44, uint32);
              memory19=flash_read(SECTOR_NUM1, 32, uint32);
            }
            
            if(memory_flag1==1)
                ;
            else            
              memory21=flash_read(SECTOR_NUM, 60, uint8);
            
            if(memory_flag2==1)
               ;
            else 
              memory22=flash_read(SECTOR_NUM, 64, uint8);
            
            if(memory_flag3==1)
                ;
            else
              memory23=flash_read(SECTOR_NUM, 68, uint16);
              
            if(memory_flag4==1)
               ;
            else              
              memory24=flash_read(SECTOR_NUM, 76, uint16);
            
            if(memory_flag5==1)
               ;
            else 
              memory25=flash_read(SECTOR_NUM, 84, uint32);
            
            if(memory_flag6==1)
               ;
            else              
               memory26=flash_read(SECTOR_NUM, 100, uint32);
            if(memory_flag9==1)
              ;
            else
              memory29=flash_read(SECTOR_NUM1, 48, uint32);
            if(menu_select!=2)
            {
              memory31=flash_read(SECTOR_NUM, 116, uint8);
              memory32=flash_read(SECTOR_NUM, 120, uint8);
              memory33=flash_read(SECTOR_NUM, 124, uint16);
              memory34=flash_read(SECTOR_NUM, 132, uint16);
              memory35=flash_read(SECTOR_NUM, 140, uint32);
              memory36=flash_read(SECTOR_NUM, 156, uint32);
              memory39=flash_read(SECTOR_NUM1, 64, uint32);
            }
            if(menu_select!=3)
            {
              memory41=flash_read(SECTOR_NUM, 172, uint8);
              memory42=flash_read(SECTOR_NUM, 176, uint8);
              memory43=flash_read(SECTOR_NUM, 180, uint16);
              memory44=flash_read(SECTOR_NUM, 188, uint32);
              memory45=flash_read(SECTOR_NUM, 196, uint32);
              memory46=flash_read(SECTOR_NUM, 212, uint32);
              memory49=flash_read(SECTOR_NUM1, 80, uint32);
            }
            if(menu_select!=4)
            {
              memory51=flash_read(SECTOR_NUM, 228, uint8);
              memory52=flash_read(SECTOR_NUM, 232, uint16);
              memory53=flash_read(SECTOR_NUM, 236, uint16);
              memory54=flash_read(SECTOR_NUM, 244, uint32);
              memory55=flash_read(SECTOR_NUM1, 0, uint32);    
              memory56=flash_read(SECTOR_NUM1, 16, uint32);
              memory59=flash_read(SECTOR_NUM1, 96, uint32);
            }  
          }
          memoryflag1=0;
        }        
        else if(menu_select==2)   //�ж�ѡ��ĵڼ����˵�
        {
          if(memoryflag1==1)   //�ж�ѡ��ĵڼ�������
          {
            memory31=VAR_VALUE(tab);   //��¼���θı��ֵ 
            
            memory_flag1=1;
          }
          else if(memoryflag1==2)
          {

            memory32=VAR_VALUE(tab);
            
            memory_flag2=1;
          }
          else if(memoryflag1==3)
          {
            memory33=VAR_VALUE(tab);
           
            memory_flag3=1;
          }
          else if(memoryflag1==4)
          {
            memory34=VAR_VALUE(tab);
            
            memory_flag4=1;
          }
          else if(memoryflag1==5)
          {
            memory35=VAR_VALUE(tab);
            
            memory_flag5=1;
          }
          else if(memoryflag1==6)
          {
            memory36=VAR_VALUE(tab); 
            
            memory_flag6=1;
          } 
          else if(memoryflag1==9)
          {
            memory39=VAR_VALUE(tab); 
            
            memory_flag9=1;          
          }
          else if(memoryflag1==8)     //��ֹʲô�����ı� �Ͱ�ȷ�� �����Ϳ���ʹÿ���˵������ݶ��ܱ�������
          {
            if(menu_select!=0)
            {
              memory11=flash_read(SECTOR_NUM, 4, uint8);
              memory12=flash_read(SECTOR_NUM, 8, uint8);
              memory13=flash_read(SECTOR_NUM, 12, uint16);
              memory14=flash_read(SECTOR_NUM, 20, uint16);
              memory15=flash_read(SECTOR_NUM, 28, uint32);
              memory16=flash_read(SECTOR_NUM, 44, uint32);
              memory19=flash_read(SECTOR_NUM1, 32, uint32);
            }
            if(menu_select!=1)
            {
              memory21=flash_read(SECTOR_NUM, 60, uint8);
              memory22=flash_read(SECTOR_NUM, 64, uint8);
              memory23=flash_read(SECTOR_NUM, 68, uint16);
              memory24=flash_read(SECTOR_NUM, 76, uint16);
              memory25=flash_read(SECTOR_NUM, 84, uint32);
              memory26=flash_read(SECTOR_NUM, 100, uint32);
              memory29=flash_read(SECTOR_NUM1, 48, uint32);
            }

            if(memory_flag1==1)
                ;
            else
              memory31=flash_read(SECTOR_NUM, 116, uint8);
            if(memory_flag2==1)
               ;
            else 
              memory32=flash_read(SECTOR_NUM, 120, uint8);
            if(memory_flag3==1)
                ;
            else
              memory33=flash_read(SECTOR_NUM, 124, uint16);
            if(memory_flag4==1)
               ;
            else      
               memory34=flash_read(SECTOR_NUM, 132, uint16);
            if(memory_flag5==1)
               ;
            else 
              memory35=flash_read(SECTOR_NUM, 140, uint32);
            if(memory_flag6==1)
               ;
            else   
              memory36=flash_read(SECTOR_NUM, 156, uint32);
            if(memory_flag9==1)
              ;
            else   
              memory39=flash_read(SECTOR_NUM1, 64, uint32);
            
            if(menu_select!=3)
            {
              memory41=flash_read(SECTOR_NUM, 172, uint8);
              memory42=flash_read(SECTOR_NUM, 176, uint8);
              memory43=flash_read(SECTOR_NUM, 180, uint16);
              memory44=flash_read(SECTOR_NUM, 188, uint32);
              memory45=flash_read(SECTOR_NUM, 196, uint32);
              memory46=flash_read(SECTOR_NUM, 212, uint32);
              memory49=flash_read(SECTOR_NUM1, 80, uint32);              
            }
            if(menu_select!=4)
            {
              memory51=flash_read(SECTOR_NUM, 228, uint8);
              memory52=flash_read(SECTOR_NUM, 232, uint16);
              memory53=flash_read(SECTOR_NUM, 236, uint16);
              memory54=flash_read(SECTOR_NUM, 244, uint32);
              memory55=flash_read(SECTOR_NUM1, 0, uint32);    
              memory56=flash_read(SECTOR_NUM1, 16, uint32);
              memory59=flash_read(SECTOR_NUM1, 96, uint32);
            }
          } 
          memoryflag1=0;
        }        
        else if(menu_select==3)    //�ж�ѡ��ĵڼ����˵�
        {
          if(memoryflag1==1)
          {
            memory41=VAR_VALUE(tab);
            memory_flag1=1;
          }
          else if(memoryflag1==2)
          {
            memory42=VAR_VALUE(tab);
            memory_flag2=1;
          }
          else if(memoryflag1==3)
          {
            memory43=VAR_VALUE(tab);
            memory_flag3=1;           
          } 
          else if(memoryflag1==4)
          {
            memory44=VAR_VALUE(tab);
            memory_flag4=1;
          } 
          else if(memoryflag1==5)
          {
            memory45=VAR_VALUE(tab);
            memory_flag5=1;
          } 
          else if(memoryflag1==6)
          {
            memory46=VAR_VALUE(tab); 
            memory_flag6=1;
          } 
          else if(memoryflag1==9)
          {
            memory49=VAR_VALUE(tab); 
            memory_flag9=1;
          }             
          else if(memoryflag1==8)     //��ֹʲô�����ı� �Ͱ�ȷ�� �����Ϳ���ʹÿ���˵������ݶ��ܱ�������
          {
            if(menu_select!=0)
            {
              memory11=flash_read(SECTOR_NUM, 4, uint8);
              memory12=flash_read(SECTOR_NUM, 8, uint8);
              memory13=flash_read(SECTOR_NUM, 12, uint16);
              memory14=flash_read(SECTOR_NUM, 20, uint16);
              memory15=flash_read(SECTOR_NUM, 28, uint32);
              memory16=flash_read(SECTOR_NUM, 44, uint32);
              memory19=flash_read(SECTOR_NUM1, 32, uint32);
            }
            if(menu_select!=1)
            {
              memory21=flash_read(SECTOR_NUM, 60, uint8);
              memory22=flash_read(SECTOR_NUM, 64, uint8);
              memory23=flash_read(SECTOR_NUM, 68, uint16);
              memory24=flash_read(SECTOR_NUM, 76, uint16);
              memory25=flash_read(SECTOR_NUM, 84, uint32);
              memory26=flash_read(SECTOR_NUM, 100, uint32);
              memory29=flash_read(SECTOR_NUM1, 48, uint32);
            }
            if(menu_select!=2)
            {
              memory31=flash_read(SECTOR_NUM, 116, uint8);
              memory32=flash_read(SECTOR_NUM, 120, uint8);
              memory33=flash_read(SECTOR_NUM, 124, uint16);
              memory34=flash_read(SECTOR_NUM, 132, uint16);
              memory35=flash_read(SECTOR_NUM, 140, uint32);
              memory36=flash_read(SECTOR_NUM, 156, uint32);
              memory39=flash_read(SECTOR_NUM1, 64, uint32);
            }
            
            if(memory_flag1==1)
                ;
            else
               memory41=flash_read(SECTOR_NUM, 172, uint8);
              ;
            if(memory_flag2==1)
               ;
            else 
               memory42=flash_read(SECTOR_NUM, 176, uint8);
            if(memory_flag3==1)
                ;
            else
               memory43=flash_read(SECTOR_NUM, 180, uint16);
            if(memory_flag4==1)
               ;
            else      
              memory44=flash_read(SECTOR_NUM, 188, uint32);
            if(memory_flag5==1)
               ;
            else 
              memory45=flash_read(SECTOR_NUM, 196, uint32); 
            if(memory_flag6==1)
               ;
            else   
              memory46=flash_read(SECTOR_NUM, 212, uint32);        
            if(memory_flag9==1)
              ;
            else   
              memory49=flash_read(SECTOR_NUM1, 80, uint32); 
            
            if(menu_select!=4)
            {
              memory51=flash_read(SECTOR_NUM, 228, uint8);
              memory52=flash_read(SECTOR_NUM, 232, uint16);
              memory53=flash_read(SECTOR_NUM, 236, uint16);
              memory54=flash_read(SECTOR_NUM, 244, uint32);
              memory55=flash_read(SECTOR_NUM1, 0, uint32);    
              memory56=flash_read(SECTOR_NUM1, 16, uint32);
              memory59=flash_read(SECTOR_NUM1, 96, uint32);              
            }
          }           
          memoryflag1=0;
        }       
        else if(menu_select==4)     //�ж�ѡ��ĵڼ����˵�
        {
          if(memoryflag1==1)
          {
            memory51=VAR_VALUE(tab);
            memory_flag1=1;
          }
          else if(memoryflag1==2)
          {
            memory52=VAR_VALUE(tab);
            memory_flag2=1;
          }
          else if(memoryflag1==3)
          {
            memory53=VAR_VALUE(tab);
            memory_flag3=1;            
          }
          else if(memoryflag1==4)
          {
            memory54=VAR_VALUE(tab);
            memory_flag4=1;           
          }
          else if(memoryflag1==5)
          {
            memory55=VAR_VALUE(tab);
            memory_flag5=1;
          }  
          else if(memoryflag1==6)
          {  
            memory56=VAR_VALUE(tab);   
            memory_flag6=1;
          } 
          else if(memoryflag1==9)
          {  
            memory59=VAR_VALUE(tab);   
            memory_flag9=1;
          } 
                    
          else if(memoryflag1==8)     //��ֹʲô�����ı� �Ͱ�ȷ�� �����Ϳ���ʹÿ���˵������ݶ��ܱ�������
          {
            if(menu_select!=0)
            {
              memory11=flash_read(SECTOR_NUM, 4, uint8);
              memory12=flash_read(SECTOR_NUM, 8, uint8);
              memory13=flash_read(SECTOR_NUM, 12, uint16);
              memory14=flash_read(SECTOR_NUM, 20, uint16);
              memory15=flash_read(SECTOR_NUM, 28, uint32);
              memory16=flash_read(SECTOR_NUM, 44, uint32);
              memory19=flash_read(SECTOR_NUM1, 32, uint32);              
            }
            if(menu_select!=1)
            {
              memory21=flash_read(SECTOR_NUM, 60, uint8);
              memory22=flash_read(SECTOR_NUM, 64, uint8);
              memory23=flash_read(SECTOR_NUM, 68, uint16);
              memory24=flash_read(SECTOR_NUM, 76, uint16);
              memory25=flash_read(SECTOR_NUM, 84, uint32);
              memory26=flash_read(SECTOR_NUM, 100, uint32);
              memory29=flash_read(SECTOR_NUM1, 48, uint32);              
            }
            if(menu_select!=2)
            {
              memory31=flash_read(SECTOR_NUM, 116, uint8);
              memory32=flash_read(SECTOR_NUM, 120, uint8);
              memory33=flash_read(SECTOR_NUM, 124, uint16);
              memory34=flash_read(SECTOR_NUM, 132, uint16);
              memory35=flash_read(SECTOR_NUM, 140, uint32);
              memory36=flash_read(SECTOR_NUM, 156, uint32);
              memory39=flash_read(SECTOR_NUM1, 64, uint32); 
            }
            if(menu_select!=3)
            {
              memory41=flash_read(SECTOR_NUM, 172, uint8);
              memory42=flash_read(SECTOR_NUM, 176, uint8);
              memory43=flash_read(SECTOR_NUM, 180, uint16);
              memory44=flash_read(SECTOR_NUM, 188, uint32);
              memory45=flash_read(SECTOR_NUM, 196, uint32);
              memory46=flash_read(SECTOR_NUM, 212, uint32);
              memory49=flash_read(SECTOR_NUM1, 80, uint32); 
            }
            
            if(memory_flag1==1)
                ;
            else
              memory51=flash_read(SECTOR_NUM, 228, uint8);
            if(memory_flag2==1)
               ;
            else 
              memory52=flash_read(SECTOR_NUM, 232, uint16); 
            if(memory_flag3==1)
                ;
            else
              memory53=flash_read(SECTOR_NUM, 236, uint16);
            if(memory_flag4==1)
               ;
            else      
              memory54=flash_read(SECTOR_NUM, 244, uint32);
            if(memory_flag5==1)
               ;
            else 
              memory55=flash_read(SECTOR_NUM1, 0, uint32);    
            if(memory_flag6==1)
               ;
            else   
              memory56=flash_read(SECTOR_NUM1, 16, uint32);     
            if(memory_flag9==1)
               ;
            else   
              memory59=flash_read(SECTOR_NUM1, 96, uint32);     
          }           
          memoryflag1=0;
        }    
     
        
        if(return_menu1==7)
           if(return_menu2==1)
             save_var((var_tab_e)tab,0);  
        
        if(fw_confirm1==8)
           if(fw_confirm2==1)
           {
              flash_erase_sector(SECTOR_NUM);    //��������  SECTOR_NUMͨ���궨������   //д��flash����ǰ����Ҫ�Ȳ�����Ӧ������(��Ȼ���ݻ���)
              flash_erase_sector(SECTOR_NUM1);
            

              flash_write(SECTOR_NUM, 4,memory11);
              flash_write(SECTOR_NUM, 8,memory12);
              flash_write(SECTOR_NUM, 12,memory13);
              flash_write(SECTOR_NUM, 20,memory14);
              flash_write(SECTOR_NUM, 28,memory15);
              flash_write(SECTOR_NUM, 44,memory16);
              flash_write(SECTOR_NUM1,32,memory19);
        
              flash_write(SECTOR_NUM, 60,memory21);        
              flash_write(SECTOR_NUM, 64,memory22);         
              flash_write(SECTOR_NUM, 68,memory23);  
              flash_write(SECTOR_NUM, 76,memory24);  
              flash_write(SECTOR_NUM, 84,memory25);  
              flash_write(SECTOR_NUM, 100,memory26); 
              flash_write(SECTOR_NUM1,48,memory29);              
              
              flash_write(SECTOR_NUM, 116,memory31);       
              flash_write(SECTOR_NUM, 120,memory32);          
              flash_write(SECTOR_NUM, 124,memory33);     
              flash_write(SECTOR_NUM, 132,memory34);    
              flash_write(SECTOR_NUM, 140,memory35);
              flash_write(SECTOR_NUM, 156,memory36);
              flash_write(SECTOR_NUM1,64,memory39);
              
               write_flag=1;
              
              flash_write(SECTOR_NUM, 172,memory41);
              flash_write(SECTOR_NUM, 176,memory42);
              flash_write(SECTOR_NUM, 180,memory43);
              flash_write(SECTOR_NUM, 188,memory44);
              flash_write(SECTOR_NUM, 196,memory45);
              flash_write(SECTOR_NUM, 212,memory46);
              flash_write(SECTOR_NUM1,80,memory49);
              
              flash_write(SECTOR_NUM, 228,memory51);
              flash_write(SECTOR_NUM, 232,memory52);              
              flash_write(SECTOR_NUM, 236,memory53);
              flash_write(SECTOR_NUM, 244,memory54);
              flash_write(SECTOR_NUM1, 0,memory55);
              flash_write(SECTOR_NUM1, 16,memory56);
              flash_write(SECTOR_NUM1,96,memory59);
             
              fw_confirm1=0;
              fw_confirm2=0;
             // VAR_VALUE(tab)=0;
              
              memory_flag1=0;
              memory_flag2=0;
              memory_flag3=0;
              memory_flag4=0;
              memory_flag5=0;
              memory_flag6=0;
              memory_flag9=0;
           }

    }
    while(i--);

    return 1;
}
uint8    car_ctrl_get()
{
    return  car_ctrl;
}


//��״̬����
uint8 car_ctrl_syn(CAR_CTRL_MODE_e mode)
{
    uint8 ret;
    ASSERT(mode < CAR_CTRL_MAX);

    VAR_VALUE(CAR_CTRL) =  mode;

    ret =  var_syn(CAR_CTRL);

    var_display(CAR_CTRL);

    return ret;
};
