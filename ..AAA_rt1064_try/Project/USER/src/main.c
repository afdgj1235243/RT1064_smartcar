/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library ����RT1064DVL6A ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� RT1064DVL6A ��Դ���һ����
* 
* RT1064DVL6A ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 8.32.4 or MDK 5.33
* ����ƽ̨          RT1064DVL6A

* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/



/***********************************************************/

#include "zf_common_headfile.h"

#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//�궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //�궨�����GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //�궨��GPIO�ж����ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�

/**********************************************************/

// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// �������ǿ�Դ����ֲ�ÿչ���


/***********************ȫ�ֱ�������*************************/
	
	bool run =1;
	
	int x_text,y_text;

	extern int32 encoder1,encoder2,encoder3,encoder4;
	
	extern int speed_tar_1;
	
	extern unsigned char original_image[image_h][image_w];
	
	extern unsigned char bin_image[image_h][image_w];
	
	extern int location_point_num;
	
	extern int8 car_test_x;
	
	extern int8 car_test_y;
	
extern location_goal Car;

extern jieshou_try_need jieshoushuju;


	int8 x_error,y_error;
	
	uint8 ceshishuzu[10]; 
	
//int8 test_location_x[7] = {-4,-2,-1,0,1,2,4};
//int8 test_location_y[7] = {-4,-2,-1,0,1,2,4};
/***********************************************************/	


void main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // ����ɾ��
    debug_init();                   // ���Զ˿ڳ�ʼ��
 
	/**********************��ģ���ʼ��*************************************************/
	
		
	
		tft180_set_dir(TFT180_CROSSWISE);
		tft180_set_font(TFT180_8X16_FONT);
		tft180_set_color(RGB565_WHITE, RGB565_BLACK);
		tft180_init();
	
		tft180_show_string(0,0,"uart_init");
//		uart_init(UART_1,115200,UART1_TX_B12,UART1_RX_B13);
//		uart_init(UART_4,115200,UART4_TX_C16,UART4_RX_C17);
	
		tft180_show_string(0,15,"lineate_uart_init");
	
		lineate_uart_init();
	
//		uart_rx_interrupt(UART_4, 1);		
//		wireless_uart_init();
//		uart_init_text();
	
		tft180_show_string(0,30,"motor_init");
		motor_init();
		
		tft180_show_string(0,45,"encord_init");
		encord_init();
		
		tft180_show_string(0,60,"icm20602_init");
		icm20602_init();
		
		
		tft180_show_string(0,75,"mt9v03x_init");
		mt9v03x_init();
		
		
		interrupt_global_enable(0);
		
		//ʹ���ж�CH0
//		pit_enable(PIT_CH0);

	/***********************************************************************************/

		
//		lineate_uart_try();
		tft180_clear();
		
		 
//		jieshou_map(24);
//		tft180_show_string(0,0,"jieshou_success");
		
	while(1){
		
//			tft180_show_uint(0,1,test_location_x[x_text],3);
//			tft180_show_uint(0,16,test_location_y[y_text],3);
			
			if(gpio_get_level(C14)==0){
			
			x_text= x_text+1;
			system_delay_ms(150);
		}
			if(gpio_get_level(C13)==0){
			
			y_text= y_text+1;
			system_delay_ms(150);	
			
		}
		
		
		if(gpio_get_level(C12)==0){
			
			tft180_clear();
			tft180_show_string(0,0,"Self test success");
			 
			while(gpio_get_level(C12)==0);
			
			uart_write_string(UART_1, "2\n");
			
//			jieshou_try(50);
			
			tft180_show_string(0,0,"send success");
			system_delay_ms(20);
			tft180_clear();
			break;
			
		}
	}

//	pit_ms_init(PIT_CH0, 1);
//	pit_ms_init(PIT_CH1, 1);
//	
		
//	
//	jieshou_map(1);
	
			car_locationread();
//	location_shortest();
		while(1)
		{
			
			
			
			fifo_text(ceshishuzu);
		
			tft180_show_uint(0,0,1,5);
			
//			      MatrixKey();
//            Menu_Scan();
			
//						move_distance();
			
//			deal_image();
//			
//			tft180_show_gray_image(0, 0, bin_image[0], image_w, image_h, image_w / 1.5, image_h / 1.5, 0);
			
			
//			for(int i=0;i<2;i++){
//				for(int j=0;j<12;j++){
//					if(i==0){
//						((j<6) ? (tft180_show_uint((i+1)*30,16*j,jieshoushuju.add[j],3)) : (tft180_show_uint((i+2)*30,16*(j-6),jieshoushuju.add[j],3)));
//					}else if(i==1){
//						((j<6) ? (tft180_show_uint((i+2)*30,16*j,jieshoushuju.add[i*12+j],3)) : (tft180_show_uint((i+3)*30,16*(j-6),jieshoushuju.add[i*12+j],3)));
//					}
//				}
//			}
//			



//	tft180_show_uint(0,0,jieshoushuju.len,3);

//	for(int i=0;i<6;i++){
//		tft180_show_uint(0,16*(i+1),jieshoushuju.X[i],3);
//		tft180_show_uint(30,16*(i+1),jieshoushuju.Y[i],3);
//	}

//	for(int i=6;i<12;i++){
//		tft180_show_uint(60,16*(i-5),jieshoushuju.X[i],3);
//		tft180_show_uint(90,16*(i-5),jieshoushuju.Y[i],3);
//	}
//	main_movement(jieshoushuju.len);



//	
//			 car_omni(3,4,Car.Speed_Z);
//motor_run(true);
//			tft180_displayimage03x(mt9v03x_image[0], 94, 60);
			
//	car_ahead();
//	motor_run(true);








//			tft180_show_uint(0,3*16,location_point_num,3);

//			
//			main_movement(jieshoushuju.len);
//				move_test(6);




//if(sqrt((2*Car.encord_add1*Car.encord_add1)+(2*Car.encord_add2*Car.encord_add2))<2*sqrt(test_location_x[x_text]*test_location_x[x_text]+test_location_y[y_text]*test_location_y[y_text])*20){

//    car_omni(test_location_x[x_text],test_location_y[y_text],Car.Speed_Z);
//}else{

//car_stop();}
//	
//	
//}

}
}

void image_find_move()
{
	
		for(int h=0;h<188;h++)
	{
		for(int w = 0;w<120;w++)
		{
			if(bin_image[h][w] == white_pixel)
			{
				if(bin_image[h+10][10] == white_pixel)
				{
					x_error = w - 60;
					y_error = h - 40;
				}
			}
			
		}
	}
	
	car_omni(y_error*0.01,x_error*0.01, Car.Speed_Z);
	
}
	




