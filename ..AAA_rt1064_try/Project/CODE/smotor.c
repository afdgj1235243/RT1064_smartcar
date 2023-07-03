#include "smotor.h"

#define SMOTOR1_PIN   PWM4_MODULE2_CHA_C30       //������̨���1����              //���峵ģѰ���������
#define SMOTOR2_PIN   PWM1_MODULE3_CHA_D0        //������̨���2����              //������̨���1����
//#define SMOTOR3_PIN   PWM1_MODULE0_CHB_D13                                       //������̨���2����
#define ELECTROMAG_PIN D1


#define SMOTOR1_CENTER  (1.5*50000/20)
#define SMOTOR2_CENTER  (1.5*50000/20)
//#define SMOTOR3_CENTER  (1.5*50000/20)


void smotor_init(void)
{
    pwm_init(SMOTOR1_PIN, 50, SMOTOR1_CENTER+1600);
    gpio_init(ELECTROMAG_PIN,GPO,0,GPI_PULL_UP);
    smotor1_control(2800);
    pwm_init(SMOTOR2_PIN, 50, SMOTOR2_CENTER);
//    pwm_init(SMOTOR3_PIN, 50, SMOTOR3_CENTER);
}

void smotor1_control(int16 duty)
{
    pwm_set_duty(SMOTOR1_PIN, (int16)SMOTOR1_CENTER + duty);
}

void smotor2_control(int16 duty)
{
    pwm_set_duty(SMOTOR2_PIN, (int16)SMOTOR1_CENTER + duty);
}

//void smotor3_control(int16 duty)
//{
//    pwm_set_duty(SMOTOR3_PIN, (int16)SMOTOR1_CENTER + duty);
//}

void grab_picture(){
    //������Ƶ��������
    gpio_set_level(ELECTROMAG_PIN,1);
    //������ԣ���Χ��Լ��-300~2800 �ȽϺ��ʣ�������ɵĶ�����������������е㲻�ȡ�
    smotor1_control(0);
    system_delay_ms(1000);
	  smotor2_control(1600);
	    system_delay_ms(1000);
    smotor1_control(2800);
	    system_delay_ms(1000);
}

void place_picture(){
    smotor2_control(0);
    //������Ƶ�����ɿ�
    system_delay_ms(100);
    gpio_set_level(ELECTROMAG_PIN,0);
    smotor2_control(1600);
}