#include "location.h"

bool panduan;

int location_point_num = 0;

extern location_goal Car;
extern jieshou_try_need jieshoushuju;

int8 x_add_test[6]={3,15,8,4,10,1};
int8 y_add_test[6]={4,1,12,10,10,8};

int8 car_test_x_last = 0,car_test_y_last = 0;
int8 car_target_x_last = 0,car_target_y_last = 0;

void car_locationread()
{
	
	jieshoushuju.len = 10*jieshoushuju.add[0]+jieshoushuju.add[1];
	for(int i=0;i<48;i=i+4)
	{
		
		jieshoushuju.X[i/4] = 10*jieshoushuju.add[i+2]+jieshoushuju.add[i+3];
		jieshoushuju.Y[i/4] = 10*jieshoushuju.add[i+4]+jieshoushuju.add[i+5];
	}
	
	
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
	int8 car_test_x,car_test_y;
	
	if(location_point_num < point){
		
			car_test_x = x_add_test[location_point_num] - car_test_x_last;
			car_test_y = y_add_test[location_point_num] - car_test_y_last;
		
			car_test_x_last = x_add_test[location_point_num];
			car_test_y_last = y_add_test[location_point_num];
		
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
			icm20602_init();
			encoder_init();
			system_delay_ms(1000);
			
			break;
		}
	}	
			 
}



void main_movement(int point)
{
	
	int8 car_target_x = 0,car_target_y = 0;
	
	if(location_point_num < point){
		
			car_target_x = jieshoushuju.X[location_point_num] - car_target_x_last;
			car_target_y = jieshoushuju.Y[location_point_num] - car_target_y_last;
			
			car_target_x_last = jieshoushuju.X[location_point_num];
			car_target_y_last = jieshoushuju.Y[location_point_num];
			
			
			location_point_num ++;
		
	}else{
		
		car_stop();
		tft180_show_uint(0,4*16,point,3);
		while(1);
	}
	
while(1){	
	
	if(sqrt((2*Car.encord_add1*Car.encord_add1)+(2*Car.encord_add2*Car.encord_add2))<2*sqrt(car_target_x*car_target_x+car_target_y*car_target_y)*20){

    car_omni(car_target_x,car_target_y,Car.Speed_Z);
		
		
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



int8  now_x = 0,now_y = 0;

void location_shortest()
{
	int  i ,j , k ,t, a, b, c, d, e, mile ,mile1 ,max;
	t=0;
	j=0;
	c=0;
	d=0;
	max=jieshoushuju.len;
	for( k = 0; k < max ; k++ )
	{
		mile = 10000;
		for( j = t ; j < max   ;j++)
		{
			mile1 = (jieshoushuju.X[j]-c)*(jieshoushuju.X[j]-c) + (jieshoushuju.Y[j]-d)*(jieshoushuju.Y[j]-d);
			if (mile1<mile)
				{
					mile=mile1;
					 a= jieshoushuju.X[t];
					 b= jieshoushuju.Y[t];
					 jieshoushuju.X[t]= jieshoushuju.X[j];
					 jieshoushuju.Y[t]= jieshoushuju.Y[j];
					 jieshoushuju.X[j]=a;
					 jieshoushuju.Y[j]=b;
				}
			
		}		
			c=jieshoushuju.X[t];
			d=jieshoushuju.Y[t];
		  t++;
	}
}



void location_swtich(int x)
{		
	now_x = jieshoushuju.X[x] - now_x;
	now_y = jieshoushuju.Y[x] - now_y;		
}