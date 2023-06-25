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
		
  /***************************����������������**************************************/		
	while(1){
		
		
		
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
			
			uart_write_string(UART_4, "2\n");
			
			tft180_show_string(0,0,"send success");
			system_delay_ms(20);
			tft180_clear();
			break;
			
		}
	}

	pit_ms_init(PIT_CH0, 1);                  //�����ж�0
/*****************����������******************/
		while(1)
		{

//			deal_image();
//			while(1){
//	send_encord(UART_1,1);}
//			send_test();
			
//	move_test(1);
			car_omni(0,1,Car.Speed_Z);
  tft180_show_float(0,0,Car.MileageX,5,5);
  tft180_show_float(0,20,Car.MileageY,5,5);
	tft180_show_float(0,40,detaxx,5,5);
	tft180_show_float(0,60,detayy,5,5);
	tft180_show_float(0,80,duty1,5,5);
	tft180_show_float(0,100,encoder3,5,5);
	tft180_show_float(50,100,RC_encoder3,5,5);
			
			
//			fifo_text();
//			image_find_move();

}
}


