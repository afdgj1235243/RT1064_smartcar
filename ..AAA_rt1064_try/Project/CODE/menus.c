/*菜单
创建日期：2023/4/18
by小天才狗蛋*/
#include "menus.h"

extern location_goal Car;

extern int32 encoder1,encoder2,encoder3,encoder4;
	
extern int32 duty1,duty2,duty3,duty4;

extern int16 RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4;
	
extern int speed_tar_1,speed_tar_2,speed_tar_3,speed_tar_4;
//引用数据


int menu_out,
    menu_out2,
    menu_out3,
    menu_out4,
    menu_out5;
int u;
float U;
unsigned char KeyNum;
void Menu_Scan(void)
{
  tft180_show_string(0, 0, "show");
	tft180_show_string(0, 15, "set");
	tft180_show_string(0, 30, "2077");
//	tft180_show_string(0, 45, "Look at the stars");
//	tft180_show_string(0, 63, "in the sky,that's");
//	tft180_show_string(0, 80, "all my wishes");
//	tft180_show_string(0, 98, "especiallly for you");
	  KeyNum = MatrixKey();
    switch(KeyNum)
        {
           //主菜单1						
					case 1:menu_out=1;
                    tft180_clear();
                    while(menu_out)
                    {
                        tft180_show_string(0,0,"1.dynamic");
                        tft180_show_string(0,20,"2.static");
                        switch(MatrixKey())
                        {
                            case 0: menu_out=0;
                            break;
                            case 1:    menu_out2=1;
                                       tft180_clear();
                                       while(menu_out2)
                                       {                
																					 showGyro();
																					 showEncoder();
																				   showDuty();
                                           menu_out2 = MatrixKey();
                                       }
                                       tft180_clear();
                            break;
														                            
														case 2:    menu_out2=1;
                                       tft180_clear();
                                       while(menu_out2)
                                       {
                                           showSpeedtar();
																				   showLUNzhuan();
                                           menu_out2 = MatrixKey();
                                       }
                                       tft180_clear();
                            break;
                        }
                    }
                    tft180_clear();
                    break;					
				            //主菜单3
            case 3: menu_out=1;

                    tft180_clear();
                    while(menu_out)
                    {
                        tft180_show_string(0,0,"I Really Want to");
									      tft180_show_string(0,15,"Stay At Your House");
                        switch(MatrixKey())
                        {
                            case 0:
                                    menu_out = 0;
                            break;                                                                                                               
                        }

                    }
                    tft180_clear();
                    break;
        }																						
}
void showGyro(void)
{
  tft180_show_string(0,0,"Gyro:");
  tft180_show_int(0,15,eulerAngle.pitch,3);
  tft180_show_int(0,30,eulerAngle.roll,3);
  tft180_show_int(0,45,eulerAngle.yaw,3);
}
void showEncoder(void)
{
  tft180_show_string(50,0,"encoder:");
  tft180_show_int(50,15,encoder1,5);
	tft180_show_int(50,30,encoder2,5);
  tft180_show_int(50,45,encoder3,5);
	tft180_show_int(50,60,encoder4,5);	
}	
void showDuty(void)
{
	tft180_show_string(0,75,"Duty:");
	tft180_show_int(0,90,duty1,5);
	tft180_show_int(50,90,duty2,5);
	tft180_show_int(0,105,duty4,5);
	tft180_show_int(50,105,duty4,5);
}
void showSpeedtar(void)
{	
  tft180_show_string(0,0,"Speedtar:");
	tft180_show_int(0,15,speed_tar_1,5);
  tft180_show_int(50,15,speed_tar_2,5);
	tft180_show_int(0,30,speed_tar_3,5);
	tft180_show_int(50,30,speed_tar_4,5);
}
void showLUNzhuan(void)
{
	tft180_show_string(0,75,"LUNz:");
	tft180_show_int(0,90,Car.encord_add1,5);
	tft180_show_int(50,90,Car.encord_add2,5);
	tft180_show_int(0,105,Car.encord_add3,5);
	tft180_show_int(50,105,Car.encord_add4,5);
}