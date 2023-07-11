#include "smotor.h"

#define SMOTOR_UPDOWN_PIN   PWM1_MODULE3_CHA_D0       //����������
#define SMOTOR_TURN_PIN  	PWM1_MODULE3_CHB_D1       	 //��ת�������


#define STEPPING_MOTOR_PIN   PWM2_MODULE3_CHA_B9   	 //�����������

#define STEPPING_MOTOR_DIR   D4 

//#define SMOTOR3_PIN   PWM4_MODULE2_CHA_C30           //���������

#define ELECTROMAG_PIN C30

#define SERVO_MOTOR_FREQ            (200 )                                           // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300
#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//�궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //�궨�����GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //�궨��GPIO�ж����ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�
//#define SMOTOR_UPDOWN_CENTER  (1.5*50000/20)
//#define SMOTOR_TURN_CENTER  (1.5*50000/20)
//#define STEPPING_MOTOR_CENTER  (1.5*50000/20)

#define SMOTOR_UPDOWN_CENTER  2000//4200
#define SMOTOR_TURN_CENTER    750//2700
#define STEPPING_MOTOR_CENTER  1500

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))
	
int16 K_enhance = 2200;

float servo_motor_duty = 1500;  //�����ǰֵ
float servo_motor_duty_addition; //����ۼ�ֵ

float smotor_KP = 1,smotor_KI,smotor_KD = 1;

uint8 stepping_judge = 1;
uint8 picture_type_buffer[100] = {0};      //����ͼƬ���ʹ洢���򾏴�
uint8 picture_type = 0;      //����ͼƬ���ʹ洢����
uint8 now_stepping_count = 1;   // ���嵱ǰ�������λ����
int8 stepping_direction = -1;    //���岽������ƶ�����

void smotor_init(void)
{
//    pwm_init(SMOTOR_UPDOWN_PIN, SERVO_MOTOR_FREQ, SMOTOR_UPDOWN_CENTER);
//    gpio_init(ELECTROMAG_PIN,GPO,0,GPI_PULL_UP);
//    smotor_updown_control(2800);
//    pwm_init(SMOTOR_TURN_PIN, SERVO_MOTOR_FREQ, SMOTOR_TURN_CENTER);
//		gpio_init(ELECTROMAG_PIN, GPO, 1, GPIO_PIN_CONFIG);
	    pwm_init(SMOTOR_UPDOWN_PIN, SERVO_MOTOR_FREQ, SMOTOR_UPDOWN_CENTER);
//    gpio_init(ELECTROMAG_PIN,GPO,0,GPI_PULL_UP);
//    smotor_updown_control(2800);
    pwm_init(SMOTOR_TURN_PIN, SERVO_MOTOR_FREQ, SMOTOR_TURN_CENTER);
	
		gpio_init(ELECTROMAG_PIN, GPO, 0, GPIO_PIN_CONFIG);
		gpio_init(STEPPING_MOTOR_DIR, GPO, 1, GPIO_PIN_CONFIG);
    pwm_init(STEPPING_MOTOR_PIN, 3000, 0);
	
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

	uart_write_string(UART_4, "3");
	system_delay_ms(1000);	
picture_read_countrol();

//    gpio_set_level(ELECTROMAG_PIN,1);
pwm_set_duty(SMOTOR_UPDOWN_PIN, 4200);                    														//2200,,1100,1050
//kaiqi daincitie
pwm_set_duty(SMOTOR_TURN_PIN, 2690);	
	
system_delay_ms(1000);
gpio_set_level(ELECTROMAG_PIN,1);//pwm_init(SMOTOR_UPDOWN_PIN, SERVO_MOTOR_FREQ, SMOTOR_UPDOWN_CENTER);
system_delay_ms(400);
pwm_set_duty(SMOTOR_UPDOWN_PIN, 1500);
	system_delay_ms(75);
////xuanzhaun
pwm_set_duty(SMOTOR_TURN_PIN, 750);
	
	system_delay_ms(1000);
	
	
	
	
	
	
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
//    smotor_turn_control(2150);
//    system_delay_ms(100);
//    gpio_set_level(ELECTROMAG_PIN,0);
//    smotor_turn_control(1600);
	 gpio_set_level(ELECTROMAG_PIN,0);
	
	system_delay_ms(1000);
	pwm_set_duty(SMOTOR_UPDOWN_PIN, 2000);                    														//2200,,1100,1050
//kaiqi daincitie
pwm_set_duty(SMOTOR_TURN_PIN, 750);	
}

/**********************�����������**********************************/
//state:using

//instance: stepping_motor_control(1);

/*************************************************************************/

void stepping_motor_control(uint16 run)
{
	if(run > 0){
	pwm_set_duty(STEPPING_MOTOR_PIN,1500);
	}else{
	pwm_set_duty(STEPPING_MOTOR_PIN,0);
	}
}


int smotor_pid(int now_position,int Target){
	
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - now_position);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=smotor_KP*Bias+smotor_KI*Integral_bias+smotor_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}

/**********************�ӻ�������ȡͼƬ���Ͳ����Ʋ������**********************************/
//state:using

//instance: picture_read_countrol(void);            //ע���������̬��ʱ��Ҫ��֤�������������

/***************************************************************************************/

void picture_read_countrol()
{
	uint8 count;
	uint8 step_duty;
	uint8 dir_duty = 1;
	lineate_uart_buff_read(picture_type_buffer,64);
	picture_type = picture_type_buffer[1] - 48;
	
	tft180_show_float(0,2*16,picture_type,3,3);
	if(picture_type != now_stepping_count)
	{
		if(picture_type < now_stepping_count && stepping_direction == 1){
			gpio_set_level(STEPPING_MOTOR_DIR,1);
			stepping_direction = -stepping_direction;
			
		}else if(picture_type > now_stepping_count && stepping_direction == -1){
			gpio_set_level(STEPPING_MOTOR_DIR,0);	
			stepping_direction = -stepping_direction;
			
		}
		
		step_duty = abs(now_stepping_count - picture_type);
		now_stepping_count = picture_type;
		stepping_motor_control(1);
//		stepping_judge = step_duty*K_enhance;
		system_delay_ms(step_command(step_duty));
		stepping_motor_control(0);
		
	}
}


uint16 step_command(uint8 step)
{
	uint16 duty;
	if(step == 1)
	{
		duty = 450;
	}
		if(step == 2)
	{
		duty = 900;
	}
		if(step == 3)
	{
		duty = 1350;
	}
		if(step == 4)
	{
		duty = 1800;
	}
		if(step == 5)
	{
		duty = 2250;
	}
	return duty;
}