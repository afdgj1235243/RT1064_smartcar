/*全局规划
2023/7/1
by 小天才狗蛋*/

#include "global_strategy.h"

int remain_Num=6;

extern int ax_add_test[12],ay_add_test[12];        
extern location_goal Car;

void remain_planning1()//最后一个坐标在中场左边
{
			int planning_Num=0;
			int planningx[11] = {34,34,1,1,34,34,1,1,34,34,1};
			int planningy[11] = {1,5,5,9,9,13,13,17,17,21,21};		
			int car_last_x=1,car_last_y=1;
			int8 car_test_x,car_test_y;
			car_test_x = 1 - ax_add_test[11];
			car_test_y = 1 - ay_add_test[11];
//这个循环是将车辆调整到1，1点			
				while(1){	
				if(abs(Car.MileageX) < 20*abs(car_test_x)){
				//if(abs(Car.MileageY) < 20*abs(car_test_y)){
					car_omni(car_test_x,car_test_y,Car.Speed_Z);			
				}
					else{
						car_stop();
						icm20602_init();
						encoder_init();
						system_delay_ms(1000);
						break;
					}
				}	
//开始进行全图搜寻
		while(remain_Num)
		{
				if(planning_Num < 11 && remain_Num > 0){					     				
					car_test_x = ax_add_test[planning_Num] - car_last_x;
					car_test_y = ay_add_test[planning_Num] - car_last_y;								
					car_last_x = ax_add_test[planning_Num];
					car_last_y = ay_add_test[planning_Num];				
					planning_Num ++;				
				}
				while(1){	
					if(car_test_x == 0){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //这是在X=0的特殊情况下使用的，不要骂我写的是shit山代码
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}		
		/*如果识别到了搬运
			加个if判断，然后--
			 remain_Num--;//识别到一个目标后--
		*/
			}
		}
}


void remain_planning2()
{
				int planning_Num=0;
			int planningx[11] = {1,1,34,34,1,1,34,34,1,1,34};
			int planningy[11] = {1,5,5,9,9,13,13,17,17,21,21};		
			int car_last_x=1,car_last_y=1;
			int8 car_test_x,car_test_y;
			car_test_x = 34 - ax_add_test[11];
			car_test_y = 1 - ay_add_test[11];
//这个循环是将车辆调整到34，1点			
				while(1){	
				if(abs(Car.MileageX) < 20*abs(car_test_x)){
					car_omni(car_test_x,car_test_y,Car.Speed_Z);			
				}
					else{
						car_stop();
						icm20602_init();
						encoder_init();
						system_delay_ms(1000);
						break;
					}
				}	
//开始进行全图搜寻
		while(remain_Num)
		{
				if(planning_Num < 11 && remain_Num > 0){					     				
					car_test_x = ax_add_test[planning_Num] - car_last_x;
					car_test_y = ay_add_test[planning_Num] - car_last_y;								
					car_last_x = ax_add_test[planning_Num];
					car_last_y = ay_add_test[planning_Num];				
					planning_Num ++;				
				}
				while(1){	
					if(car_test_x == 0){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //这是在X=0的特殊情况下使用的，不要骂我写的是shit山代码
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}		
		/*如果识别到了搬运
			加个if判断，然后--
			 remain_Num--;//识别到一个目标后--
		*/
			}
		}
}

void remain()     //固定坐标识别完成，开始搜寻散落无坐标点图片*****这个库的主函数
{
	 if(ax_add_test[11] < 18)
	{
		remain_planning1();
	}else{
		remain_planning2();
	}		
}