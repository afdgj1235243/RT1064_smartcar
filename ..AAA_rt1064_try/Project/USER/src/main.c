/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A

* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/



/***********************************************************/

#include "zf_common_headfile.h"

#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//宏定义GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //宏定义快速GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //宏定义GPIO中断引脚的默认配置，便于初始化GPIO中断时快速填写参数，如果需要其他参数可自行修改

/**********************************************************/

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程


/***********************全局变量定义*************************/
	
	bool run =1;
	
	int x_text,y_text;

	extern int32 encoder1,encoder2,encoder3,encoder4;
	
	extern int speed_tar_1;
	
	extern unsigned char original_image[image_h][image_w];
	
	
	extern int location_point_num;
	
	extern int8 car_test_x;
	
	extern int8 car_test_y;
	
  extern location_goal Car;

  extern jieshou_try_need jieshoushuju;
	
	extern float detaxx,detayy;
	extern float MileageX,MileageY;
	extern int32 duty1;
	extern int32 encoder1;
	extern int32 RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4;
	uint8 ceshishuzu[10]; 
	
	extern double speed_tar_1_fix ;
	extern double speed_tar_2_fix ;
  extern double speed_tar_3_fix ;
  extern double speed_tar_4_fix ;
	
	extern int ax_add_test[12];
	extern int ay_add_test[12];
	
	extern int x_add_test[12];
	extern int y_add_test[12];
	
/***********************************************************/	


void main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
 
	/**********************各模块初始化*************************************************/
	
		
	
		tft180_set_dir(TFT180_CROSSWISE);
		tft180_set_font(TFT180_8X16_FONT);
		tft180_set_color(RGB565_WHITE, RGB565_BLACK);
		tft180_init();
	
		tft180_show_string(0,0,"data_send_init");
		data_send_uart_init();
	
		tft180_show_string(0,15,"lineate_uart_init");
	
		lineate_uart_init();
	
		tft180_show_string(0,30,"motor_init");
		motor_init();
		
		tft180_show_string(0,45,"encord_init");
		encord_init();
		
		tft180_show_string(0,60,"icm20602_init");
		icm20602_init();
		
		
		tft180_show_string(0,75,"mt9v03x_init");
		mt9v03x_init();
		
		
		interrupt_global_enable(0);
	

	/***********************************************************************************/

		tft180_clear();
		
  /***************************按键进入跳车界面**************************************/		
	while(1){
		
		
		
//			if(gpio_get_level(C14)==0){
//			
//			x_text= x_text+1;
//			system_delay_ms(150);
//		}
//			if(gpio_get_level(C13)==0){
//			
//			y_text= y_text+1;
//			system_delay_ms(150);	
//			
//		}
		
		
		if(gpio_get_level(C12)==0){
			
			tft180_clear();
			tft180_show_string(0,0,"Self test success");
			 
			while(gpio_get_level(C12)==0);
			
			uart_write_string(UART_4, "2");
	
			tft180_show_string(0,0,"send success");
			
				system_delay_ms(500);			           //延时保证接收全部数据
				location_point_read();
//			for(int i = 0;i < 12;i++)
//			{
//				ax_add_test[i] = jieshoushuju.X[i];
//				ay_add_test[i] = jieshoushuju.Y[i];
//			}
//				path();
//			for(int i = 0;i < 12;i++)
//			{
//				jieshoushuju.X[i] = ax_add_test[i];
//				jieshoushuju.Y[i] = ay_add_test[i];
//			}
//			system_delay_ms(20);
			tft180_clear();
//			
			
tft180_show_uint(0,0,jieshoushuju.len,3);

	for(int i=0;i<6;i++){
		tft180_show_uint(0,16*(i+1),jieshoushuju.X[i],3);
		tft180_show_uint(30,16*(i+1),jieshoushuju.Y[i],3);
	}

	for(int i=6;i<12;i++){
		tft180_show_uint(60,16*(i-5),jieshoushuju.X[i],3);
		tft180_show_uint(90,16*(i-5),jieshoushuju.Y[i],3);
	}

//	for(int i=0;i<6;i++){
//		tft180_show_uint(0,16*(i+1),ax_add_test[i],3);
//		tft180_show_uint(30,16*(i+1),ay_add_test[i],3);
//	}

//	for(int i=6;i<12;i++){
//		tft180_show_uint(60,16*(i-5),ax_add_test[i],3);
//		tft180_show_uint(90,16*(i-5),ay_add_test[i],3);
//	}
//				path();
			break;
			
		}
		
	}


	
	while(gpio_get_level(C14)==1);
	system_delay_ms(1000);
	
	pit_ms_init(PIT_CH0, 1);                  //开启中断0
/*****************主函数运行******************/

//		for (int i = 0; i < 12; i++) {
//    ax_add_test[i] =(int) jieshoushuju.X[i];
//    ay_add_test[i] =(int) jieshoushuju.Y[i];
//    }

//		for (int i = 0; i < 12; i++) {
//    ax_add_test[i] = x_add_test[i];
//    ay_add_test[i] = x_add_test[i];
//    }
	
		tft180_clear();
//		 path();
		
//	    for (int i = 0; i < 6; i++) {
//			tft180_show_int(0,15*i,x_add_test[i],3);
//			tft180_show_int(30,15*i,y_add_test[i],3);
//			tft180_show_int(60,15*i,x_add_test[i+6],3);
// 			tft180_show_int(90,15*i,y_add_test[i+6],3);
//	    }
//			while(1);
			
		while(1)
		{
			
//			
//	for(int i=0;i<6;i++){
//		tft180_show_uint(0,16*(i+1),jieshoushuju.X[i],3);
//		tft180_show_uint(30,16*(i+1),jieshoushuju.Y[i],3);
//	}

//	for(int i=6;i<12;i++){
//		tft180_show_uint(60,16*(i-5),jieshoushuju.X[i],3);
//		tft180_show_uint(90,16*(i-5),jieshoushuju.Y[i],3);
//	}
	

//			deal_image();
//			while(1){
//	send_encord(UART_1,1);}
//			send_test();
//			
//	move_test(1);
//			
//	tft180_show_float(0,2*16,speed_tar_1_fix,3,3);
//	tft180_show_float(0,3*16,speed_tar_2_fix,3,3);
//	tft180_show_float(0,4*16,speed_tar_3_fix,3,3);
//	tft180_show_float(0,5*16,speed_tar_4_fix,3,3);
			
//			
//			car_ahead();
//			car_omni(0,3,Car.Speed_Z);
			
////			
//  tft180_show_float(0,0,Car.MileageX,5,5);
////			while(1);
//  tft180_show_float(0,20,Car.MileageY,5,5);
//	tft180_show_float(0,40,detaxx,5,5);
//	tft180_show_float(0,60,detayy,5,5);
//	tft180_show_float(50,60,encoder1,5,5);		
//	tft180_show_float(0,80,RC_encoder1,5,5);
//	tft180_show_float(50,80,RC_encoder2,5,5);
//	tft180_show_float(0,100,RC_encoder3,5,5);
//	tft180_show_float(50,100,RC_encoder4,5,5);
//	
	
//			main_movement_new(jieshoushuju.len);
       move_test(12);

//			fifo_text();
//			image_find_move();

}
}


