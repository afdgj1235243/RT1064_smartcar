#include "smotor.h"

#define SMOTOR_TURN_PIN   PWM1_MODULE3_CHA_D0       //����������
#define SMOTOR_UPDOWN_PIN  	PWM1_MODULE3_CHB_D1       	 //��ת�������

#define STEPPING_MOTOR_PIN   PWM1_MODULE0_CHB_D13   	 //�����������
//#define STEPPING_MOTOR_DIR   PWM4_MODULE2_CHA_C30  

//#define SMOTOR3_PIN   PWM4_MODULE2_CHA_C30           //���������

#define ELECTROMAG_PIN C30

#define SERVO_MOTOR_FREQ            (200 )                                           // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300
#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//�궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //�궨�����GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //�궨��GPIO�ж����ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�
//#define SMOTOR_UPDOWN_CENTER  (1.5*50000/20)
//#define SMOTOR_TURN_CENTER  (1.5*50000/20)
//#define STEPPING_MOTOR_CENTER  (1.5*50000/20)

#define SMOTOR_UPDOWN_CENTER  2100
#define SMOTOR_TURN_CENTER  1200
#define STEPPING_MOTOR_CENTER  1500

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))
	
float servo_motor_duty = 1500;  //�����ǰֵ
float servo_motor_duty_addition; //����ۼ�ֵ

float smotor_KP = 1,smotor_KI,smotor_KD = 1;
void smotor_init(void)
{
    pwm_init(SMOTOR_UPDOWN_PIN, SERVO_MOTOR_FREQ, SMOTOR_UPDOWN_CENTER);
//    gpio_init(ELECTROMAG_PIN,GPO,0,GPI_PULL_UP);
//    smotor_updown_control(2800);
    pwm_init(SMOTOR_TURN_PIN, SERVO_MOTOR_FREQ, SMOTOR_TURN_CENTER);
		gpio_init(ELECTROMAG_PIN, GPO, 1, GPIO_PIN_CONFIG);
//    pwm_init(STEPPING_MOTOR_PIN, 50, STEPPING_MOTOR_CENTER);
	
}
/********************����������*******************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/


void smotor_updown_control(uint32 target_angle)                                    
{
	uint32 duty;
		while(servo_motor_duty != target_angle)
	{
		if(servo_motor_duty < target_angle)
		{
			duty = smotor_pid(servo_motor_duty,target_angle);
			
			pwm_set_duty(SMOTOR_UPDOWN_PIN, (uint32)SMOTOR_UPDOWN_CENTER + duty);
			
			servo_motor_duty += duty;
		}else{
			duty = smotor_pid(servo_motor_duty,target_angle);
			
			pwm_set_duty(SMOTOR_UPDOWN_PIN, (uint32)SMOTOR_UPDOWN_CENTER + duty);
			
			servo_motor_duty -= duty;
		}
	}
}


/********************��ת�������*******************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/

void smotor_turn_control(int16 duty)
{
    pwm_set_duty(SMOTOR_TURN_PIN, (int16)SMOTOR_TURN_CENTER + duty);
}



//void smotor3_control(int16 duty)
//{
//    pwm_set_duty(SMOTOR3_PIN, (int16)SMOTOR1_CENTER + duty);
//}

/********************ͼƬץȡ����*******************************/
//state:testing

//instance: main_movement(point_lens);
/**************************************************************/

void grab_picture(){
	

//    gpio_set_level(ELECTROMAG_PIN,1);
pwm_set_duty(SMOTOR_UPDOWN_PIN, 4400);                    														//2200,,1100,1050
//kaiqi daincitie
system_delay_ms(1000);
gpio_set_level(ELECTROMAG_PIN,1);//pwm_init(SMOTOR_UPDOWN_PIN, SERVO_MOTOR_FREQ, SMOTOR_UPDOWN_CENTER);
system_delay_ms(1000);
pwm_set_duty(SMOTOR_UPDOWN_PIN, 2100);
	system_delay_ms(1000);
////xuanzhaun
//pwm_set_duty(SMOTOR_UPDOWN_PIN, 1050);
	
	
	
	
	
	
	
	
//    smotor_updown_control(1700);
	
//    system_delay_ms(1000);
//	smotor_updown_control(1500);
//	  smotor_updown_control(1500);
//	  smotor_turn_control(1600);
//	    system_delay_ms(1000);
//    smotor_updown_control(2800);
//	    system_delay_ms(1000);
}

void place_picture(){
    smotor_turn_control(2150);
    system_delay_ms(100);
    gpio_set_level(ELECTROMAG_PIN,0);
    smotor_turn_control(1600);
}

void stepping_motor_control()
{
	
}


int smotor_pid(int now_position,int Target){
	
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - now_position);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=smotor_KP*Bias+smotor_KI*Integral_bias+smotor_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}
