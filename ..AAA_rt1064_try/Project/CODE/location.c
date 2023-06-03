#include "location.h"

bool panduan;

int location_point_num = 0;

extern location_goal Car;
extern jieshou_try_need jieshoushuju;

int8 x_add_test[4]={3,2,5,-4};
int8 y_add_test[4]={4,1,3,8};

int8 car_test_x,car_test_y;

void car_locationread()
{
	
}


void location_lines(int8 x)
{
	int8 i = 0;
		do 
		{
			move_test(jieshoushuju.X[i],jieshoushuju.Y[i]);
			i++;
		}while(i<x);
	
}


void move_test(int point)
{
	
	if(location_point_num < point){
		
			car_test_x = x_add_test[location_point_num];
			car_test_y = y_add_test[location_point_num];
			location_point_num ++;
		
	}else{
		car_stop();
		tft180_show_uint(0,4*16,point,3);
		while(1);
	}
	
while(1){	
	if(sqrt((2*Car.encord_add1*Car.encord_add1)+(2*Car.encord_add2*Car.encord_add2))<2*sqrt(car_test_x*car_test_x+car_test_y*car_test_y)*20){

    car_omni(car_test_x,car_test_y,Car.Speed_Z);
		
		
	}
		else{
			
//			tft180_show_uint(0,1,car_test_x,3);
//			tft180_show_uint(0,16,car_test_y,3);
			
			car_stop();
			encoder_init();
			system_delay_ms(1000);
			
			break;
		}
	}	
			 
}
