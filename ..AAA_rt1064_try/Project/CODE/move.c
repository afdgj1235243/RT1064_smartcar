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


//前轮接口
#define PWM_1 PWM2_MODULE3_CHB_D3		
#define PWM_2 PWM2_MODULE2_CHB_C11

//dv8701后轮接口
#define PWM_3 PWM2_MODULE1_CHB_C9
#define PWM_4 PWM2_MODULE0_CHB_C7

////2014驱动后轮接口
//#define PWM_5 PWM2_MODULE1_CHA_C8		
//#define PWM_6 PWM2_MODULE0_CHA_C6

//#define PWM_7 PWM2_MODULE1_CHB_C9
//#define PWM_8 PWM2_MODULE0_CHB_C7

#define MAX_DUTY            (50             )  

#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//宏定义GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //宏定义快速GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //宏定义GPIO中断引脚的默认配置，便于初始化GPIO中断时快速填写参数，如果需要其他参数可自行修改

//#define PWM_CH1             (PWM1_MODULE3_CHB_D1)

float Position_KP =200;
float Position_KI =20;
float Position_KD =5;

//电机目标速度
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

int32 duty1=0,duty2=0,duty3=0,duty4=0;//电机pwm值

double speed_tar = 10;//目标速度



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
// 函数简介     测试函数
// 参数说明     void
// 返回参数     void
// 使用示例    	try();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

//void try()
//{
//     pwm_init(PWM_1, 17000, 0);                                                // PWM 通道 1 初始化频率 17KHz 占空比初始为 0
//    pwm_init(PWM_4, 17000, 0);                                                // PWM 通道 2 初始化频率 17KHz 占空比初始为 0
//    
//    interrupt_global_enable(0);
//    
//     while(1)
//    {
//        if(duty >= 0)                                                           // 左侧正转
//        {
//            pwm_set_duty(PWM_1, duty * (PWM_DUTY_MAX / 100));                 // 计算占空比
//            pwm_set_duty(PWM_4, 0);                                           // 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
//        }
//        else                                                                    // 左侧反转
//        {
//            pwm_set_duty(PWM_1, 0);                                           // 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
//            pwm_set_duty(PWM_4, (-duty) * (PWM_DUTY_MAX / 100));              // 计算占空比
//        }
//        if(dir)                                                                 // 根据方向判断计数方向 本例程仅作参考
//        {
//            duty ++;                                                            // 正向计数
//            if(duty >= MAX_DUTY)                                                // 达到最大值
//            dir = false;                                                        // 变更计数方向
//        }
//        else
//        {
//            duty --;                                                            // 反向计数
//            if(duty <= -MAX_DUTY)                                               // 达到最小值
//            dir = true;                                                         // 变更计数方向
//        }
//        system_delay_ms(50);
//    }
//}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     车身运动函数
// 参数说明     void
// 返回参数     void
// 使用示例     car_ahead();
// 备注信息     
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
// 函数简介     车身运动函数第二版本
// 参数说明     void
// 返回参数     void
// 使用示例     car_ahead(speed);
// 备注信息     后面看一下是不是用预定义的speed会好一些
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




//位置PID

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     位置式pid
// 参数说明     int
// 返回参数     int
// 使用示例     position_pid1(encoder1,speed_tar_1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------


int position_pid1(int Encoder,int Target){
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}

int position_pid2(int Encoder,int Target){
	
	
    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}

int position_pid3(int Encoder,int Target){
	

    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}


int position_pid4(int Encoder,int Target){
	

    static float Bias,Pwm,Integral_bias,Last_Bias;
	
    Bias=(float)(Target - Encoder);//当前误差
	
    Integral_bias+=Bias;//误差和
	
    Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	
    Last_Bias=Bias;
	
    return (int)Pwm;
}





//-------------------------------------------------------------------------------------------------------------------
// 函数简介     pid限幅
// 参数说明     void
// 返回参数     void
// 使用示例     limit_pid_pwm(duty1);
// 备注信息     
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
// 函数简介     pid计算函数，以达到目标速度
// 参数说明     void
// 返回参数     void
// 使用示例     pid_calculate();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------


void pid_calculate(void){
	
    duty1 = position_pid1(encoder1,speed_tar_1);    //后期车出现问题时注意一下这里的正负号问题
    duty2 = position_pid2(-encoder2,speed_tar_2);
    duty3 = position_pid3(encoder3,speed_tar_3);
    duty4 = position_pid4(-encoder4,speed_tar_4);
	  
		
		duty1 = limit_pid_pwm(duty1);
    duty2 = limit_pid_pwm(duty2);
    duty3 = limit_pid_pwm(duty3);
    duty4 = limit_pid_pwm(duty4);
	
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     车身驱动函数，判断是否开始启动，如返回run则启动函数计算pid（dv8701代码）
// 参数说明     void
// 返回参数     void
// 使用示例     motor_run(run);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

//当前为2104与8701混合

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




////2104控制驱动代码

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
//前偏右1
	if(x>0&&y>0&&x<y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar*((y-x)/(y+x)) - z;
			speed_tar_3= -speed_tar*((y-x)/(y+x)) - z;
			speed_tar_4= -speed_tar + z; 
		}
//前偏左1
	if(x<0&&y>0&&-x<y)
		{			
			speed_tar_1= speed_tar*((y-(-x))/(y+(-x))) + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar*((y-(-x))/(y+(-x))) + z; 				
		}
//左偏前1
	if(x<0&&y>0&&-x>y)
		{
			speed_tar_1= speed_tar*((y-(-x))/(y+(-x))) + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar*((y-(-x))/(y+(-x))) + z; 		
		}
//左偏后1
	if(x<0&&y<0&&-x>-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= -speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_3= speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_4= speed_tar + z; 			
		}
//右偏后
	if(x>0&&y<0&&x>-y)
		{
			speed_tar_1= -speed_tar*((-y-x)/(-y+x)) + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= speed_tar*((-y-x)/(-y+x)) + z; 
		}
//右偏前1
	if(x>0&&y>0&&x>y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar*((y-x)/(y+x)) - z;
			speed_tar_3= -speed_tar*((y-x)/(y+x)) - z;
			speed_tar_4= -speed_tar + z; 			
		}
//后偏右
	if(x>0&&y<0&&x<-y)
		{
			speed_tar_1= -speed_tar*((-y-x)/(-y+x)) + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= speed_tar*((-y-x)/(-y+x)) + z; 
		}
//后偏左
	if(x<0&&y<0&&-x<-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= -speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_3= speed_tar*((-y-(-x))/(-y+(-x))) - z;
			speed_tar_4= speed_tar + z; 			
		}
//正前
	if(x==0&&y>0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}
//正右		
	if(x>0&&y==0)
		{			
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}
//正左		
	if(x<0&&y==0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= -speed_tar + z; 
		}	
//正后		
	if(x==0&&y<0)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= -speed_tar - z;
			speed_tar_4= speed_tar + z; 
		}
//正右前		
	if(x>0&&y>0&&x==y)
		{
			speed_tar_1= speed_tar + z;
			speed_tar_2= 0- z;
			speed_tar_3= 0 - z;
			speed_tar_4= -speed_tar + z; 
		}
//正右后		
	if(x>0&&y<0&&x==-y)
		{
			speed_tar_1= 0 + z;
			speed_tar_2= -speed_tar - z;
			speed_tar_3= speed_tar - z;
			speed_tar_4= 0 + z; 
		}
//正左后		
	if(x<0&&y<0&&-x==-y)
		{
			speed_tar_1= -speed_tar + z;
			speed_tar_2= 0 - z;
			speed_tar_3= 0 - z;
			speed_tar_4= speed_tar + z; 
		}
//正左前		
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