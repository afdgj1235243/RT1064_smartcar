#include "location.h"

#define LOCATION_TARGETX  20
#define LOCATION_TARGETY  100

bool panduan;

int location_point_num = 0;

extern location_goal Car;
extern jieshou_try_need jieshoushuju;
extern location_now location_correct;

int8 x_add_test[12]={32,10,14,20,8,13,18,8,15,23,32,26};
int8 y_add_test[12]={3,4,6,6,8,10,13,14,16,16,17,19};

int8 car_test_x_last = 1,car_test_y_last = 0;
int8 car_target_x_last = 1,car_target_y_last = 0;

int8 point_x_middle;                       //判断空白色块中点坐标值
int8 point_y_middle;

uint8_t location_correct_target_x = 140;    //限定坐标
uint8_t location_correct_target_y = 90;
extern double speed_tar;//目标速度
float location_KP = 0.007,location_KI = 0,location_KD = 0.0005; //坐标定位位置式PID

extern uint8 concern_point_x[300];
extern uint8 concern_point_y[300];

/********************坐标读取*******************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/
void car_locationread()
{
	
	jieshoushuju.len = 10*jieshoushuju.add[0]+jieshoushuju.add[1];
	for(int i=0;i<48;i=i+4)
	{
		
		jieshoushuju.X[i/4] = 10*jieshoushuju.add[i+2]+jieshoushuju.add[i+3];
		jieshoushuju.Y[i/4] = 10*jieshoushuju.add[i+4]+jieshoushuju.add[i+5];
	}
	
	
}

/********************运动测试函数*******************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/
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
		
//		tft180_show_uint(0,4*16,point,3);
		while(1);
	}
	
while(1){	
	if(abs(Car.MileageX) < 20*abs(car_test_x)){
//if(abs(Car.MileageY) < 20*abs(car_test_y)){
    car_omni(car_test_x,car_test_y,Car.Speed_Z);
		
		
	}
		else{
			
//			tft180_show_uint(0,1,car_test_x,3);
//			tft180_show_uint(0,16,car_test_y,3);
			  tft180_show_float(0,0,Car.MileageX,5,5);

				tft180_show_float(0,20,Car.MileageY,5,5);
			
			car_stop();
			location_correct_text();
			icm20602_init();
			encoder_init();
			system_delay_ms(1000);
			
			break;
		}
	}	
			 
}

/********************走已知点**********************************/
//state:useing

//instance: main_movement(point_lens);
/**************************************************************/

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
			uart_write_string(UART_4, "3\n");
			
			system_delay_ms(1000);
			
			break;
		}
	}	
			 
}


/********************走已知点_新里程计**********************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/

void main_movement_new(int point)
{
	bool x_flag=false,y_flag=false;
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
	
	if(abs(Car.MileageX)<abs(car_target_x)*20){
		
    car_omni(car_target_x,car_target_y,Car.Speed_Z);
		
	}
		else{
			
			car_stop();
			location_correct_text();
			encoder_init();
//			uart_write_string(UART_4, "3\n");
			
			system_delay_ms(1000);
			
			break;
		}
	}	
			 
}



//data need:
int8  now_x = 0,now_y = 0;
/********************路径规划函数*******************************/
//author:zhaoyouming

//state:testing

//instance:location_shortest();
/**************************************************************/
void location_shortest()
{
	int  i ,j , k ,t, a, b, c, d, e, mile ,mile1 , mile0, max;
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
			mile0 = jieshoushuju.X[j] + jieshoushuju.Y[j];
			mile1 = (jieshoushuju.X[j]-c)*(jieshoushuju.X[j]-c) + (jieshoushuju.Y[j]-d)*(jieshoushuju.Y[j]-d);
			if (mile1<mile&&mile0>1)
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

/********************坐标转换函数*******************************/
//state:useing

//instance:
/**************************************************************/

void location_swtich(int x)
{		
	now_x = jieshoushuju.X[x] - now_x;
	now_y = jieshoushuju.Y[x] - now_y;		
}


/********************反馈定位测试*******************************/
//state:testing

//instance:
/**************************************************************/

extern unsigned char bin_image[image_h_bin][image_w_bin];

void image_find_move()
{
	int8 x_error = 0,y_error = 0;
	
	int h_point = 0;
	int w_point = 0;
	
	while(1){
//		tft180_clear();
		deal_image();

		
		tft180_show_gray_image(0, 0, bin_image[0], image_w_bin, image_h_bin, image_w_bin, image_h_bin, 0);
		
		tft180_draw_point(concern_point_x[0], concern_point_y[0], RGB565_RED);
		tft180_draw_point(concern_point_x[0] +1, concern_point_y[0], RGB565_RED);
		tft180_draw_point(concern_point_x[0]+1, concern_point_y[0]+1, RGB565_RED);
		tft180_draw_point(concern_point_x[0], concern_point_y[0]+1, RGB565_RED);
		
//tft180_draw_point(1,1,RGB565_RED);
		for(int h=0;h<60;h++)
	{
		for(int w = 0;w<94;w++)
		{
			if(bin_image[h][w] == white_pixel)
			{
				h_point = h;
				w_point = w;
				
//				tft180_show_string(80,16*6,"fir find");
//				tft180_show_int(50,16*6,h_point,3);
//				tft180_show_int(50,16*7,w_point,3);
				
				
				
				tft180_show_uint(80,16*6,concern_point_x[0],3);
				tft180_show_uint(80,16*7,concern_point_y[0],3);
				
				tft180_show_uint(100,16*6,concern_point_x[1],3);
				tft180_show_uint(100,16*7,concern_point_y[1],3);
				
//				tft180_draw_point(1,1,RGB565_RED);
				
				x_error = location_correct_pid_x(LOCATION_TARGETX,h);
				
				y_error = location_correct_pid_y(LOCATION_TARGETY,w);
				
//			  tft180_show_int(80,16*6,x_error,3);
//				tft180_show_int(80,16*7,y_error,3);
				
			h = 60;
			}
//		 tft180_draw_line(w_point, h_point, w_point, 0,RGB565_RED); 
//		 tft180_draw_line(w_point, h_point, 0, h_point,RGB565_RED); 
//				if(boold_judue(w,h))
//				{
//					x_error = location_correct_pid_x(LOCATION_TARGETX,h);
//					y_error = location_correct_pid_y(LOCATION_TARGETY,w);
//					tft180_show_int(50,16*7,x_error,3);
//					while(1);
////					while(1){
////					deal_image();
////					tft180_show_gray_image(0, 0, bin_image[0], image_w, image_h, image_w / 1.5, image_h / 1.5, 0);}
//					car_omni(x_error,y_error, Car.Speed_Z);
//					
//				}
			}
		}
	}
	
	
	if(y_error>0){
	tft180_show_string(0,16*6,"forword");
	}else if(y_error<0){
	tft180_show_string(0,16*6,"back   ");
	}else{
	tft180_show_string(0,16*6,"stop   ");
	}
	if(x_error>0){
	tft180_show_string(0,16*7,"left ");
	}else if(x_error<0){
	tft180_show_string(0,16*7,"right");
	}else{
	tft180_show_string(0,16*7,"stop ");
	}

}



/********************色块识别与中点获取*******************************/
//state:testing (issue)

//instance:boold_judue(h,w);
/**************************************************************/

bool boold_judue(int8 image_judge_point_x,int8 image_judge_point_y)
{
	int8 point_x_first = image_judge_point_x;
	int8 point_y_first = image_judge_point_y;
	
		point_x_middle = image_judge_point_x;
		point_y_middle = image_judge_point_y;
	

	if(bin_image[image_judge_point_y][image_judge_point_x+1] == white_pixel)         //简单的超前判断，通过起始点向x，y两侧判断直到出现黑色像素，计算中点值
			{
			image_judge_point_x++;
			}else{
				point_x_middle = (point_x_middle + image_judge_point_x - 1)/2;
			}
			tft180_show_int(0,16*6,point_x_middle,3);
			tft180_show_int(80,16*3,image_judge_point_x,3);
	if(bin_image[image_judge_point_y+1][image_judge_point_x] == white_pixel)
			{
			image_judge_point_y++;
			}else{
				point_y_middle = (point_y_middle + image_judge_point_y -1)/2;
			}
			tft180_show_int(0,16*7,point_y_middle,3);
			tft180_show_int(80,16*4,image_judge_point_y,3);
if(abs(point_x_middle - point_x_first)>=1 && abs(point_y_middle - point_y_first) >= 1){
	
		 tft180_draw_line(point_x_first, point_y_first, point_x_first, 0,RGB565_RED); 
		 tft180_draw_line(point_x_first, point_y_first, 0, point_y_first,RGB565_RED); 
	
		 tft180_show_int(80,0,point_x_first,3);
	
		 tft180_show_int(80,16*1,point_y_first,3);
		return true;
}else{
return false;
}
} 

/********************定位pid计算********************************/

//state:testing

//instance:location_correct_pid(locaton_target,now_location);

/**************************************************************/
int16 location_correct_pid_x(int16 locaton_target_x,int16 now_location_x)
{
	  static float Bias,Location_point_x,Integral_bias,Last_Bias;
	
    Bias=(float)(locaton_target_x - now_location_x);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Location_point_x=location_KP*Bias+location_KI*Integral_bias+location_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Location_point_x;
}



int16 location_correct_pid_y(int16 locaton_target_y,int16 now_location_y)
{
	  static float Bias,Location_point_y,Integral_bias,Last_Bias;
	
    Bias=(float)(locaton_target_y - now_location_y);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Location_point_y=location_KP*Bias+location_KI*Integral_bias+location_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Location_point_y;
}

int16 location_correct_pid_x_and_y(int16 locaton_target_x_and_y,int16 now_location_x_and_y)
{
	  static float Bias,Location_point_x_and_y,Integral_bias,Last_Bias;
	
    Bias=-(float)(locaton_target_x_and_y - now_location_x_and_y);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Location_point_x_and_y=location_KP*Bias+location_KI*Integral_bias+location_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Location_point_x_and_y;
}
/************************微调*************************************************/

void location_correct_text()
{
	while(1)
	{
		float x = 0,y = 0;
		
		uart_write_string(UART_4, "4");
		location_correct.k = 90 / 12.0;
		if(location_correct_point_read())
		{
			x = (location_correct.x - location_correct_target_x) * location_correct.k;
			y = (location_correct.y - location_correct_target_y) * (-location_correct.k);
			speed_tar = location_correct_pid_x_and_y(0,sqrt(x*x + y*y));
			car_omni(x,y,Car.Speed_Z);
			if(speed_tar == 0)
			{
		
				car_stop();
				break;
			}
		}
//		运动部分	
//		tft180_show_uint(0,0,location_correct.x ,5);
//		tft180_show_uint(0,20,location_correct.y ,5);
//		while(1)
//		{	
		
//		}
//		if(abs(x)<5 && abs(y)<5)
//		{
//			
//			car_stop();
//			break;
//		}
		
			
		tft180_show_uint(0,0,location_correct.x ,5);
		tft180_show_uint(0,20,location_correct.y ,5);
//	
	}
	
	
	

}

