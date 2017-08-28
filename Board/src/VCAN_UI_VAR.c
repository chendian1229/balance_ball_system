#include "common.h"
#include "VCAN_LCD.h"
#include "VCAN_UI_VAR.h"
#include "VCAN_NRF24L0_MSG.h"
#include "include.h"

#define VAR_VALUE(var_tab)      num_info[var_tab].val       //指定标号的变量的值
#define VAR_OLDVALUE(var_tab)   num_info[var_tab].oldval    //指定标号的变量的最后确认值
#define VAR_MAXVALUE(var_tab)   num_info[var_tab].maxval
#define VAR_MINVALUE(var_tab)   num_info[var_tab].minval
#define VAR_SITE(var_tab)       num_info[var_tab].site

static uint8  car_ctrl = CAR_NULL;
extern uint8  var1, var2;
extern uint16  var3,var4;
extern uint32 var5, var6, var7,var8,var9;

uint32 *var_addr[VAR_MAX] = {(uint32 *)&car_ctrl,(uint32 *)&var1, (uint32 *)&var2, (uint32 *)&var3, (uint32 *)&var4, (uint32 *)&var5, (uint32 *)&var6, (uint32 *)&var7, (uint32 *)&var8, (uint32 *)&var9};

//VAR_MAX为变量数目
ui_var_info_t  num_info[VAR_MAX] =
{

    //val,oldval,会在调用key_event_init的时候从其对应变量里赋值过来，所以这里的值可以随意写
    //需要设置minval,maxval,{x,y}
    //务必注意最小值不要大于最大值
  
//  {val,oldval,minval,maxval,{x,y}}
    {0, 0, 0, CAR_CTRL_MAX, {103,102}},      //变量 car_ctrl，
    {0, 0, 0, 200, {103, 0}},                //变量 var1，
    {0, 0, 0, 200, {103, 17}},               //变量 var2，
    {0, 0, 0, 800, {103, 34}},               //变量 var3，
    {0, 0, 0, 1000, {103, 51}},               //变量 var4，
    {0, 0, 0, 500, {103, 68}},             //变量 var5，
    {0, 0, 0, 500, {103, 85}},              //变量 var6，
    {0, 0, 0, 500, {0, 102}},               //变量 var7，
    {0, 0, 0, 500, {50, 102}},               //变量 var8，
    {0, 0, 0, 500, {103, 102}}               //变量 var8，
};

uint8   new_tab = 0;        //当前选择的变量编号
uint32  last_tab;           //最后接收到的变量编号



//同步指定的值。tab 为 VAR_NUM 时表示全部同步，小于则同步对应的
//必须先同步再显示全部，因为有可能同步失败。
//static uint8 var_syn(uint8 tab);         //同步数据，1表示成功，0表示失败

void save_var2buff(var_tab_e var_num, uint8 *sendbuf);              //把需要发送的变量的数据写入到缓冲区

void var_init()
{
    uint8   var_num;
    uint32  vartemp;

    for(var_num = 0; var_num < VAR_MAX; var_num++)
    {
        get_var((var_tab_e)var_num, &vartemp);
        num_info[var_num].val       = vartemp;
        num_info[var_num].oldval    = vartemp;

        //检测最小值与最大值
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
 
//对变量的加减进行处理  VAR_ADD
void var_value(ui_var_event_e ctrl)
{
    ASSERT(new_tab < VAR_MAX);


    //修改当前变量的值
    switch(ctrl)    //判断输入值
    {
      case VAR_ADD:       //输入值为 加1  
        if(VAR_VALUE(new_tab) < VAR_MAXVALUE(new_tab)) //现在的值小于最大值加1
        {
            VAR_VALUE(new_tab)++;
        }
        else              //否则等于最小值
        {
            VAR_VALUE(new_tab) = VAR_MINVALUE(new_tab);       //最大值加1为最小值
        }
        break;

    case VAR_SUB:       //输入值为 减1  
        if(VAR_VALUE(new_tab) > VAR_MINVALUE(new_tab))   //现在的值大于最小值减1
        {
            VAR_VALUE(new_tab)--;
        }
        else             //否则等于最大值
        {
            VAR_VALUE(new_tab) = VAR_MAXVALUE(new_tab) ;//最小值减一为最大值
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
            VAR_VALUE(new_tab) = VAR_MAXVALUE(new_tab) ;//最小值减一为最大值
        }
        break;

    default:                        //无效选择，不需要切换
        break;
    }
    
    return_menu1=new_tab;
    fw_confirm1=new_tab;
    
    memoryflag1=new_tab;

    var_display(new_tab);
}

//对变量进行选择
void var_select(ui_var_event_e  ctrl)
{
    ASSERT(new_tab < VAR_MAX);

    uint8 old_tab = new_tab;       //先备份当前变量标号

    //切换到下一个变量
    switch(ctrl)
    {
    case VAR_NEXT:                      //下一个
        new_tab++;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = 0;               //从头开始
        }
        break;

    case VAR_PREV:                      //上一个
        new_tab--;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = VAR_MAX - 1;     //从尾开始
        }
        break;

    case VAR_NEXT_HOLD:                 //快下
        new_tab += VAR_SELECT_HOLD_OFFSET;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = 0;               //从头开始
        }
        break;

    case VAR_PREV_HOLD:                 //快上
        new_tab -= VAR_SELECT_HOLD_OFFSET;
        if(new_tab >= (VAR_MAX) )
        {
            new_tab = VAR_MAX - 1;     //从尾开始
        }
        break;

    default:                        //无效选择，不需要切换
        return;
    }
    
    
    var_display(old_tab);               //处理上一个变量

    var_display(new_tab);              //处理当前变量：

}


//确认当前选择的
void var_ok()
{
    ASSERT(new_tab < VAR_MAX);

    //比较是否有改变值
    if(VAR_VALUE(new_tab) != VAR_OLDVALUE(new_tab))   //值改变了，则需要处理
    {
        var_syn(new_tab);          //同步新的值
    }

    var_display(new_tab);
}

//取消当前选择的值  OK
void val_cancel()
{
    ASSERT(new_tab < VAR_MAX);

    //直接还原当前值
    VAR_VALUE(new_tab) = VAR_OLDVALUE(new_tab);

    var_display(new_tab);
}



//显示指定的值。tab 为 VAR_MAX 时表示全部显示，小于则显示对应的

void var_display(uint8 tab)
{
#if UI_VAR_USE_LCD
/*
    //定义背景延时
#define SELECT_NO_CHANGE_BG         WHITE   //当前选中，而且没有改变
#define SELECT_CHANGE_BG            WHITE   //当前选中，而且改变了
#define NO_SELECT_NO_CHANGE_BG      RED     //没有选中，而且没有改变（普通的就是这样）
#define NO_SELECT_CHANGE_BG         RED     //没有选中，而且改变了

    //定义文字颜色
#define SELECT_NO_CHANGE            BLUE    //当前选中，而且没有改变
#define SELECT_CHANGE               GREEN   //当前选中，而且改变了
#define NO_SELECT_NO_CHANGE         BLUE    //没有选中，而且没有改变（普通的就是这样）
#define NO_SELECT_CHANGE            GREEN   //没有选中，而且改变了
*/
    //定义背景延时
#define SELECT_NO_CHANGE_BG         YELLOW   //当前选中，而且没有改变
#define SELECT_CHANGE_BG            YELLOW   //当前选中，而且改变了
#define NO_SELECT_NO_CHANGE_BG      BLACK     //没有选中，而且没有改变（普通的就是这样）
#define NO_SELECT_CHANGE_BG         BLACK     //没有选中，而且改变了

    //定义文字颜色
#define SELECT_NO_CHANGE            WHITE    //当前选中，而且没有改变
#define SELECT_CHANGE               GREEN   //当前选中，而且改变了
#define NO_SELECT_NO_CHANGE         WHITE    //没有选中，而且没有改变（普通的就是这样）
#define NO_SELECT_CHANGE            GREEN   //没有选中，而且改变了  
  
  
    uint8  i = 0;
    uint16 bkColor;
    uint16 Color;

    ASSERT((new_tab < VAR_MAX) && (tab <= VAR_MAX));

    if(tab == VAR_MAX)      //显示全部
    {
        i = VAR_MAX - 1;    //循环的次数
        tab = 0;
    }

    do
    {
        if(tab == new_tab)
        {
            //显示当前的值：判断值是否改变
            if(VAR_VALUE(tab) == VAR_OLDVALUE(tab)) //值没改变，不需要处理
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
            //显示非当前的值
            if(VAR_VALUE(tab) == VAR_OLDVALUE(tab)) //值没改变，不需要处理
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

        //显示文字
        LCD_num_C(VAR_SITE(tab), VAR_VALUE(tab), Color, bkColor);

        tab++;
    }
    while(i--);         //tab != VAR_MAX 的时候，执行一次就跳出
#else
    tab = tab;          //消除编译警告
#endif
}

void save_var2buff(var_tab_e var_num, uint8 *sendbuf)
{
    uint32 temp;
    get_var(var_num, &temp);
    *((uint32 *)&sendbuf[COM_LEN]) = (uint32)var_num;
    *((uint32 *)&sendbuf[COM_LEN + sizeof(uint32)]) = temp;
}

//同步指定的值。tab 为 VAR_MAX 时表示全部同步，小于则同步对应的
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
        oldvalue = VAR_OLDVALUE(tab);                   //备份旧的值

        //把值复制到对应的变量
        save_var((var_tab_e)tab, VAR_VALUE(tab));
        
        fw_confirm2=VAR_VALUE(tab);
        return_menu2=VAR_VALUE(tab);
       
        if(menu_select==0)      //判断选择的第几个菜单
        {   
          
          if(memoryflag1==1)   //判断选择的第几个按键
          {     
            memory11=VAR_VALUE(tab);  //记录本次改变的值 
            
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
          else if(memoryflag1==8)     //防止什么都不改变 就按确定 这样就可以使每个菜单中数据都能保留下来
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
        else if(menu_select==1)   //判断选择的第几个菜单
        {
          if(memoryflag1==1)       //判断选择的第几个按键
          {
            memory21=VAR_VALUE(tab);  //记录本次改变的值 
           /* memory22=flash_read(SECTOR_NUM, 64, uint8);   //未改变的值直接从flash中读入 防止本个菜单中的数据写入错误
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
          else if(memoryflag1==8)     //防止什么都不改变 就按确定 这样就可以使每个菜单中数据都能保留下来
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
        else if(menu_select==2)   //判断选择的第几个菜单
        {
          if(memoryflag1==1)   //判断选择的第几个按键
          {
            memory31=VAR_VALUE(tab);   //记录本次改变的值 
            
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
          else if(memoryflag1==8)     //防止什么都不改变 就按确定 这样就可以使每个菜单中数据都能保留下来
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
        else if(menu_select==3)    //判断选择的第几个菜单
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
          else if(memoryflag1==8)     //防止什么都不改变 就按确定 这样就可以使每个菜单中数据都能保留下来
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
        else if(menu_select==4)     //判断选择的第几个菜单
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
                    
          else if(memoryflag1==8)     //防止什么都不改变 就按确定 这样就可以使每个菜单中数据都能保留下来
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
              flash_erase_sector(SECTOR_NUM);    //擦除扇区  SECTOR_NUM通过宏定义来的   //写入flash数据前，需要先擦除对应的扇区(不然数据会乱)
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


//车状态控制
uint8 car_ctrl_syn(CAR_CTRL_MODE_e mode)
{
    uint8 ret;
    ASSERT(mode < CAR_CTRL_MAX);

    VAR_VALUE(CAR_CTRL) =  mode;

    ret =  var_syn(CAR_CTRL);

    var_display(CAR_CTRL);

    return ret;
};
