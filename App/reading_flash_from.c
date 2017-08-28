#include "include.h"
#include "common.h"

void reading_flash_from_secotr()
{
  extern uint8 target_mode;
  extern uint8 A;extern uint8 B;extern uint8 C;extern uint8 D;
  extern float kp;extern float kd;extern uint8 allow_change;
  extern float Angle_Px;extern float Angle_Dx;extern float Angle_Py;extern float Angle_Dy;
  if(menu_select==0)
  {
    var1=flash_read(SECTOR_NUM, 4, uint8);
    var2=flash_read(SECTOR_NUM, 8, uint8);
    var3=flash_read(SECTOR_NUM, 12, uint16);
    var4=flash_read(SECTOR_NUM, 20, uint16);
    var5=flash_read(SECTOR_NUM, 28, uint32);
    var6=flash_read(SECTOR_NUM, 44, uint32);
    var9=flash_read(SECTOR_NUM1, 32, uint32);
  }
  else if(menu_select==1)
  {
     var1=flash_read(SECTOR_NUM, 60, uint8);
     var2=flash_read(SECTOR_NUM, 64, uint8);
     var3=flash_read(SECTOR_NUM, 68, uint16);
     var4=flash_read(SECTOR_NUM, 76, uint16);
     var5=flash_read(SECTOR_NUM, 84, uint32);
     var6=flash_read(SECTOR_NUM, 100, uint32);
     var9=flash_read(SECTOR_NUM1, 48, uint32);
  }
  else if(menu_select==2)
  {
     var1=flash_read(SECTOR_NUM, 116, uint8);
     var2=flash_read(SECTOR_NUM, 120, uint8);
     var3=flash_read(SECTOR_NUM, 124, uint16);
     var4=flash_read(SECTOR_NUM, 132, uint16);
     var5=flash_read(SECTOR_NUM, 140, uint32);
     var6=flash_read(SECTOR_NUM, 156, uint32);
     var9=flash_read(SECTOR_NUM1, 64, uint32);
  }
  else if(menu_select==3)
  {
    var1=flash_read(SECTOR_NUM, 172, uint8);
    var2=flash_read(SECTOR_NUM, 176, uint8);
    var3=flash_read(SECTOR_NUM, 180, uint16);
    var4=flash_read(SECTOR_NUM, 188, uint16);
    var5=flash_read(SECTOR_NUM, 196, uint32);
    var6=flash_read(SECTOR_NUM, 212, uint32);
    var9=flash_read(SECTOR_NUM1, 80, uint32);
  }   
  else if(menu_select==4)
  {
    var1=flash_read(SECTOR_NUM, 228, uint8);
    var2=flash_read(SECTOR_NUM, 232, uint8);
    var3=flash_read(SECTOR_NUM, 236, uint16);
    var4=flash_read(SECTOR_NUM, 244, uint16);
    var5=flash_read(SECTOR_NUM1, 0, uint32);
    var6=flash_read(SECTOR_NUM1, 16, uint32);
    var9=flash_read(SECTOR_NUM1, 96, uint32);
  }
  
  /**************************************************/
  target_mode=flash_read(SECTOR_NUM, 4, uint8);
  A=flash_read(SECTOR_NUM, 8, uint8);
  B=flash_read(SECTOR_NUM, 12, uint16);
  C=flash_read(SECTOR_NUM, 20, uint16);
  D=flash_read(SECTOR_NUM, 28, uint32);
  /************************************************/
  kp=flash_read(SECTOR_NUM, 60, uint8);
  kd=flash_read(SECTOR_NUM, 64, uint8);  
  allow_change=flash_read(SECTOR_NUM, 68, uint16);
  
//  Angle_Px=-(float)flash_read(SECTOR_NUM, 116, uint8)/1000;
//  Angle_Dx=-(float)flash_read(SECTOR_NUM, 120, uint8)/10;
//  Angle_Py=-(float)flash_read(SECTOR_NUM, 116, uint8)/1000;
//  Angle_Dy=-(float)flash_read(SECTOR_NUM, 120, uint8)/10;
    
}