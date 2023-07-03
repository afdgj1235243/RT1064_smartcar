//
//author:czy
//

#include "move.h"


#define DIR_1 D2
#define DIR_2 C10
#define DIR_3 C8
#define DIR_4 C6

//#define PWM_LIMIT 10000

//#define PWM_1 PWM1_MODULE3_CHB_D1		
//#define PWM_2 PWM2_MODULE3_CHB_D3

//#define PWM_3 PWM1_MODULE3_CHA_D0
//#define PWM_4 PWM4_MODULE0_CHA_B24

//#define PWM_5 PWM1_MODULE0_CHB_D13		
//#define PWM_6 PWM1_MODULE1_CHB_D15

//#define PWM_7 PWM1_MODULE0_CHA_D12
//#define PWM_8 PWM1_MODULE1_CHA_D14

#define PWM_LIMIT 8000


//ǰ�ֽӿ�
#define PWM_1 PWM2_MODULE3_CHB_D3		
#define PWM_2 PWM2_MODULE2_CHB_C11

//dv8701���ֽӿ�
#define PWM_3 PWM2_MODULE1_CHB_C9
#define PWM_4 PWM2_MODULE0_CHB_C7

////2014�������ֽӿ�
//#define PWM_5 PWM2_MODULE1_CHA_C8		
//#define PWM_6 PWM2_MODULE0_CHA_C6

//#define PWM_7 PWM2_MODULE1_CHB_C9
//#define PWM_8 PWM2_MODULE0_CHB_C7

#define MAX_DUTY            (50             )  

#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//�궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //�궨�����GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //�궨��GPIO�ж����ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�

//#define PWM_CH1             (PWM1_MODULE3_CHB_D1)

float Position_KP =200;
float Position_KI =20;
float Position_KD =5;

//���Ŀ���ٶ�
double speed_tar_1 = 0;
double speed_tar_2 = 0;
double speed_tar_3 = 0;
double speed_tar_4 = 0;

double speed_tar_1_fix  = 0;
double speed_tar_2_fix  = 0;
double speed_tar_3_fix  = 0;
double speed_tar_4_fix  = 0;

int8 duty = 0;
bool dir = true;

int32 duty1=0,duty2=0,duty3=0,duty4=0;//���pwmֵ

double speed_tar = 10;//Ŀ���ٶ�



void motor_init(void)
{
	
		gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 	
    gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG);
		gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);       
    gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(PWM_1, 17000, 0);      					
    pwm_init(PWM_2, 17000, 0);     						
    pwm_init(PWM_3, 17000, 0);                          
    pwm_init(PWM_4, 17000, 0);    
//    pwm_init(PWM_5, 17000, 0);                          
//    pwm_init(PWM_6, 17000, 0);   
//		pwm_init(PWM_7, 17000, 0);                          
//    pwm_init(PWM_8, 17000, 0); 	
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���Ժ���
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��    	try();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

//void try()
//{
//     pwm_init(PWM_1, 17000, 0);                                                // PWM ͨ�� 1 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
//    pwm_init(PWM_4, 17000, 0);                                                // PWM ͨ�� 2 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
//    
//    interrupt_global_enable(0);
//    
//     while(1)
//    {
//        if(duty >= 0)                                                           // �����ת
//        {
//            pwm_set_duty(PWM_1, duty * (PWM_DUTY_MAX / 100));                 // ����ռ�ձ�
//            pwm_set_duty(PWM_4, 0);                                           // ͬһʱ�� һ�����ֻ�����һ��PWM ��һͨ�����ֵ͵�ƽ
//        }
//        else                                                                    // ��෴ת
//        {
//            pwm_set_duty(PWM_1, 0);                                           // ͬһʱ�� һ�����ֻ�����һ��PWM ��һͨ�����ֵ͵�ƽ
//            pwm_set_duty(PWM_4, (-duty) * (PWM_DUTY_MAX / 100));              // ����ռ�ձ�
//        }
//        if(dir)                                                                 // ���ݷ����жϼ������� �����̽����ο�
//        {
//            duty ++;                                                            // �������
//            if(duty >= MAX_DUTY)                                                // �ﵽ���ֵ
//            dir = false;                                                        // �����������
//        }
//        else
//        {
//            duty --;                                                            // �������
//            if(duty <= -MAX_DUTY)                                               // �ﵽ��Сֵ
//            dir = true;                                                         // �����������
//        }
//        system_delay_ms(50);
//    }
//}


//-------------------------------------------------------------------------------------------------------------------
// �������     �����˶�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     car_ahead();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------


void car_ahead(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}


void car_back(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}


void car_turnleft(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_turnright(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = speed_tar;
}

void car_diagonal(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = speed_tar;
    speed_tar_4 = 0;
}

void car_turnround(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_anticlockwise() {
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_concerning(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

void car_stop(){
    speed_tar_1 = 0;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����˶������ڶ��汾
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     car_ahead(speed);
// ��ע��Ϣ     ���濴һ���ǲ�����Ԥ�����speed���һЩ
//-------------------------------------------------------------------------------------------------------------------


void car_ahead_new(int speed_tar_new){
    speed_tar_1 = speed_tar_new;
    speed_tar_2 = speed_tar_new;
    speed_tar_3 = speed_tar_new;
    speed_tar_4 = speed_tar_new;
}


void car_back_new(int speed_tar_new){
    speed_tar_1 = -speed_tar_new;
    speed_tar_2 = -speed_tar_new;
    speed_tar_3 = -speed_tar_new;
    speed_tar_4 = -speed_tar_new;
}


void car_turnleft_new(int speed_tar_new){
    speed_tar_1 = speed_tar_new;
    speed_tar_2 = -speed_tar_new;
    speed_tar_3 = speed_tar_new;
    speed_tar_4 = -speed_tar_new;
}

void car_turnright_new(int speed_tar_new){
    speed_tar_1 = -speed_tar_new;
    speed_tar_2 = speed_tar_new;
    speed_tar_3 = -speed_tar_new;
    speed_tar_4 = speed_tar_new;
}

void car_diagonal_new(int speed_tar_new){
    speed_tar_1 = speed_tar_new;
    speed_tar_2 = 0;
    speed_tar_3 = speed_tar_new;
    speed_tar_4 = 0;
}

void car_turnround_new(int speed_tar_new){
    speed_tar_1 = speed_tar_new;
    speed_tar_2 = speed_tar_new;
    speed_tar_3 = -speed_tar_new;
    speed_tar_4 = -speed_tar_new;
}

void car_anticlockwise_new(int speed_tar_new) {
    speed_tar_1 = -speed_tar_new;
    speed_tar_2 = -speed_tar_new;
    speed_tar_3 = speed_tar_new;
    speed_tar_4 = speed_tar_new;
}

void car_concerning_new(int speed_tar_new){
    speed_tar_1 = speed_tar_new;
    speed_tar_2 = speed_tar_new;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

void car_stop_new(){
    speed_tar_1 = 0;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}




//λ��PID

//-------------------------------------------------------------------------------------------------------------------
// �������     λ��ʽpid
// ����˵��     int
// ���ز���     int
// ʹ��ʾ��     position_pid1(encoder1,speed_tar_1);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------


int position_pid1(int Encoder,int Target){
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}

int position_pid2(int Encoder,int Target){
	
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}

int position_pid3(int Encoder,int Target){
	

    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}


int position_pid4(int Encoder,int Target){
	

    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//��ǰ���
	
    Integral_bias+=Bias;//����
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}





//-------------------------------------------------------------------------------------------------------------------
// �������     pid�޷�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     limit_pid_pwm(duty1);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------




int limit_pid_pwm(int32 duty_in)
{
		if(duty_in > PWM_LIMIT) 
		{
			duty_in = PWM_LIMIT ;
		}
		if(duty_in < -PWM_LIMIT) 
		{
			duty_in = -PWM_LIMIT ;
		}
		return duty_in;
}



//-------------------------------------------------------------------------------------------------------------------
// �������     pid���㺯�����ԴﵽĿ���ٶ�
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     pid_calculate();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------


void pid_calculate(void){
	
    duty1 = position_pid1(encoder1,speed_tar_1);    //���ڳ���������ʱע��һ�����������������
    duty2 = position_pid2(-encoder2,speed_tar_2);
    duty3 = position_pid3(encoder3,speed_tar_3);
    duty4 = position_pid4(-encoder4,speed_tar_4);
	  
		
		duty1 = limit_pid_pwm(duty1);
    duty2 = limit_pid_pwm(duty2);
    duty3 = limit_pid_pwm(duty3);
    duty4 = limit_pid_pwm(duty4);
	
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ���������������ж��Ƿ�ʼ�������緵��run��������������pid��dv8701���룩
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     motor_run(run);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

//��ǰΪ2104��8701���

void motor_run(bool run)
{
		if(run)
		{
			   pid_calculate();
    }else{
        duty1 = 0;
        duty2 = 0;
        duty3 = 0;
        duty4 = 0;
    }
if(duty1>=0){

      gpio_set_level(DIR_1,1);
			pwm_set_duty(PWM_1,duty1);
    } else {
      gpio_set_level(DIR_1,0);
			pwm_set_duty(PWM_1,-duty1);
    }
		if(duty2>=0){
   
      gpio_set_level(DIR_2,1);
			pwm_set_duty(PWM_2,duty2);
    } else {
        
      gpio_set_level(DIR_2,0);
			pwm_set_duty(PWM_2,-duty2);
    }
		 if(duty3>=0){
			 
			gpio_set_level(DIR_3,1);
			pwm_set_duty(PWM_3,duty3);
    } else {
			
      gpio_set_level(DIR_3,0);
			pwm_set_duty(PWM_3,-duty3);
    }

    if(duty4>=0){
			gpio_set_level(DIR_4,1);
			pwm_set_duty(PWM_4,duty4);
    } else {

      gpio_set_level(DIR_4,0);
			pwm_set_duty(PWM_4,-duty4);
    }
	
}




////2104������������

//void motor_control(bool run)
//{
//    if(run) {
//        pid_calculate();
//    }else{
//        duty1 = 0;
//        duty2 = 0;
//        duty3 = 0;
//        duty4 = 0;
//    }

////    if(duty1>=0){
////        
////        pwm_set_duty(PWM_1,duty1);
////			pwm_set_duty(PWM_2, 0);
////    } else {
////        
////        pwm_set_duty(PWM_1,-duty1);
////			pwm_set_duty(PWM_2, 0);
////    }
////    if(duty2>=0){
////        
////        pwm_set_duty(PWM_3,duty2);
////    } else {
////        
////        pwm_set_duty(PWM_4,-duty2);
////    }

//    if(duty3>=0){
//        
//      pwm_set_duty(PWM_6,duty3);
//			pwm_set_duty(PWM_5, 0);
//    } else {
//       
//      pwm_set_duty(PWM_6,0);
//			pwm_set_duty(PWM_5, -duty3);
//    }

//    if(duty4>=0){
//        
//      pwm_set_duty(PWM_7,duty4);
//			pwm_set_duty(PWM_8, 0);
//    } else {
//        
//      pwm_set_duty(PWM_7,0);
//			pwm_set_duty(PWM_8, -duty4);
//    }
//}

///////////////////////
void car_omni(float x, float y, float z){
//ǰƫ��1
	if(x>0&&y>0&&x<y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar*((y-x)/(y+x)) - z;
			speed_tar_3= -speed_tar*((y-x)/(y+x)) - z;
			speed_tar_4= -speed_tar + z; 
		}
//ǰƫ��1
	if(x<0&&y>0&&-x<y)
		{			
			speed_tar_1= speed_tar*((y-(-x))/(y+(-x))) + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar*((y-(-x))/(y+(-x))) + z; 				
		}
//��ƫǰ1
	if(x<0&&y>0&&-x>y)
		{
			speed_tar_1= speed_tar*((y-(-x))/(y+(-x))) + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar*((y-(-x))/(y+(-x))) + z; 		
		}
//��ƫ��1
	if(x<0&&y<0&&-x>-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= -speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_3= speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_4= speed_tar + z; 			
		}
//��ƫ��
	if(x>0&&y<0&&x>-y)
		{
			speed_tar_1= -speed_tar*((-y-x)/(-y+x)) + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= speed_tar*((-y-x)/(-y+x)) + z; 
		}
//��ƫǰ1
	if(x>0&&y>0&&x>y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar*((y-x)/(y+x)) - z;
			speed_tar_3= -speed_tar*((y-x)/(y+x)) - z;
			speed_tar_4= -speed_tar + z; 			
		}
//��ƫ��
	if(x>0&&y<0&&x<-y)
		{
			speed_tar_1= -speed_tar*((-y-x)/(-y+x)) + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= speed_tar*((-y-x)/(-y+x)) + z; 
		}
//��ƫ��
	if(x<0&&y<0&&-x<-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= -speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_3= speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_4= speed_tar + z; 			
		}
//��ǰ
	if(x==0&&y>0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}
//����		
	if(x>0&&y==0)
		{			
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}
//����		
	if(x<0&&y==0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}	
//����		
	if(x==0&&y<0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= speed_tar + z; 
		}
//����ǰ		
	if(x>0&&y>0&&x==y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= 0- z;
			speed_tar_3= 0 - z;
			speed_tar_4= -speed_tar + z; 
		}
//���Һ�		
	if(x>0&&y<0&&x==-y)
		{
			speed_tar_1= 0 + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= 0 + z; 
		}
//�����		
	if(x<0&&y<0&&-x==-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= 0 - z;
			speed_tar_3= 0 - z;
			speed_tar_4= speed_tar + z; 
		}
//����ǰ		
	if(x<0&&y>0&&-x==y)
		{
			speed_tar_1= 0 + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= 0 + z; 
		}		
		speed_tar_1_fix = speed_tar_1;
		speed_tar_2_fix = speed_tar_2;
		speed_tar_3_fix = speed_tar_3;
		speed_tar_4_fix = speed_tar_4;
}