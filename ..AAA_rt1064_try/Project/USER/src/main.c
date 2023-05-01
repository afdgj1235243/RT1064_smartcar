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
	
	extern int32 encoder1,encoder2,encoder3,encoder4;
	
	extern int speed_tar_1;
	
	extern jieshou_try_need jieshoushuju;
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
	
	
		uart_init(UART_4,115200,UART4_TX_C16,UART4_RX_C17);
	
//		lineate_uart_init();
//	while(1){tft180_show_string(0,60,"lineate_uart_init");}
		motor_init();
		encord_init();
		icm20602_init();
	
		interrupt_global_enable(0);
		
		//ʹ���ж�CH0
//		pit_enable(PIT_CH0);

	/***********************************************************************************/
	
	

		pit_ms_init(PIT_CH0, 1);
		
//		lineate_uart_try();
//		jieshou_try(100);
		
		while(1)
		{
			
//			uart_tx_interrupt(UART_1, 1);
//			try();
//			encoder_get();
//			
//			tft180_show_int(0,0,jieshoushuju.add[10],5);
//			tft180_show_int(0,15,jieshoushuju.add[20],5);
//			tft180_show_int(0,30,jieshoushuju.add[30],5);
//			tft180_show_int(0,45,jieshoushuju.add[40],5);
			
//			car_ahead();
			
//			motor_run(run);
//			motor_control(1);
//			gpio_set_level(DIR_1,1);
//			pwm_set_duty(PWM_1,2000);
//			gpio_set_level(DIR_2,1);
//			pwm_set_duty(PWM_2,2000);
//			
//			pwm_set_duty(PWM_5,1800);
//			pwm_set_duty(PWM_6,0);
//			
//			pwm_set_duty(PWM_7,1800);
//			pwm_set_duty(PWM_8,0);
//			car_ahead();
//			tft180_show_int(0,60,speed_tar_1,5);
//		motor_run(1);

//			tft180_show_string(0,60,"lineate_uart_init");
//			
//			uart_write_string(UART_4, "seekfree"); 

			      MatrixKey();
            Menu_Scan();
			
//			uart_write_byte(UART_4, 0xA5); 
			
		}



}
