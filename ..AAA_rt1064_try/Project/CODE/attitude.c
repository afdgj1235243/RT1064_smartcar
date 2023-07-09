#include "attitude.h"


extern int32 encoder1,encoder2,encoder3,encoder4;//引用编码器的值
extern float yaw;
extern double speed_tar;

int32  coord[50] = {0};


int angelTarget,kp,ki,kd,r;
float mid=0;//中值
int Outup;//输出

/////////////////////////////////////////////
uint16 msecond=0,time_second=0;
uint32 getAngel_times=0;
float pictureP=0.15f,pictureI=0,pictureD=0.03f;
float Angel_KP = 0.35f;float Angel_KD = 1.5f;float Angel_KI = 0;
double getAngel_k=0;
double getAngelN=0;
float getAngel_Err=0.00003f;
int nextpoint=0;
extern int16 count_time;

enum openart_mode{
    get_map,
    get_picture
};

enum openart_mode openartMode=get_map;

location_goal Car={0}; //小车状态（位置，目标）存储结构体

///////////////////////////////////////////////
int picture_xerror_pid(int16 now_x,int16 target_x){
    static float Bias,Speed_X,Integral_bias,Last_Bias;
    Bias=(float)(target_x - now_x);
    Integral_bias+=Bias;
    Speed_X=-pictureP*Bias+pictureI*Integral_bias+pictureD*(Bias-Last_Bias);
    Last_Bias=Bias;
    if(Speed_X>=10)
        Speed_X=10;
    if(Speed_X<=-10)
        Speed_X=-10;
    return (int)Speed_X;
}

int picture_yerror_pid(int16 now_y,int16 target_y){
    static float Bias,Speed_Y,Integral_bias,Last_Bias;
    Bias=(float)(target_y - now_y);
    Integral_bias+=Bias;
    Speed_Y=pictureP*Bias+pictureI*Integral_bias+pictureD*(Bias-Last_Bias);
    Last_Bias=Bias;
    if(Speed_Y>=10)
        Speed_Y=10;
    if(Speed_Y<=-10)
        Speed_Y=-10;
    return (int)Speed_Y;
}

double angel_pid(int NowAngel,int TargetAngel){
    if (NowAngel<=0){
        if(NowAngel-TargetAngel<=-180){
            NowAngel+=180;
            TargetAngel-=180;
        }
    }else if(NowAngel>0){
        if(NowAngel-TargetAngel>=180){
            NowAngel-=180;
            TargetAngel+=180;
        }
    }
    static float Bias,Speed_Z,Integral_bias,Last_Bias;
    Bias=(float)(TargetAngel - NowAngel);
    Integral_bias+=Bias;
    Speed_Z=Angel_KP*Bias+Angel_KI*Integral_bias+Angel_KD*(Bias-Last_Bias);
    Last_Bias=Bias;
    if(Speed_Z>=10)			
        Speed_Z=10;
    if(Speed_Z<=-10)
        Speed_Z=-10;
    return (double)Speed_Z;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     姿态调整函数，使车头始终朝前
// 参数说明     void
// 返回参数     
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void keep_Front(void)
{
    static uint32 time;
    msecond++;

    time++;
    if( 0==(time_second%1000)){
        time_second++;
    }
    getAngel_times++;

    //采集陀螺仪数据 
    //Mahony_computeAngles();
    ICM_getEulerianAngles();

    eulerAngle.yaw=eulerAngle.yaw+getAngel_Err*getAngel_times;
    Car.Angel=eulerAngle.yaw  - count_time/4;

    //获取编码器的值
    encoder_get();

    Car.Speed_Z=-angel_pid(Car.Angel,angelTarget);//omnimove模式下目标方向一直为0

//    car_omni(0,0,Car.Speed_Z);
//		
//    //控制电机转动
//    motor_run(true);
    
}

void car_omnimove(){
    bool x_flag=false,y_flag=false;
    angelTarget=0;
    if(abs(Car.MileageX)<abs(Car.DistanceX)){
        nextpoint++;
        Car.Speed_X=(speed_tar * sin(Car.Angel_Target/180 *PI));//((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
        if(Car.Speed_X>-1&&Car.Speed_X<0){
            Car.Speed_X=-1;
        }
        if(Car.Speed_X>0&&Car.Speed_X<1){
            Car.Speed_X=1;
        }
        x_flag=false;
    }else{
        Car.Speed_X=0;
        x_flag=true;
    }
    if(abs(Car.MileageY)<abs(Car.DistanceY)){
        Car.Speed_Y=(speed_tar * cos(Car.Angel_Target/180 *PI));
        if(Car.Speed_Y<0&&Car.Speed_Y>-1){
            Car.Speed_Y=-1;
        }
        if(Car.Speed_Y>0&&Car.Speed_Y<1){
            Car.Speed_Y=1;
        }
        y_flag=false;
    }else{
        Car.Speed_Y=0;
        y_flag=true;
    }

    //Car.Speed_Z=-angel_pid(  Car.Angel,-Car.Angel_Target);//速度环

//    if(x_flag && y_flag){
//        car_stop();
//        reset_mileage();
//        car_recmode();

//        //rt_mb_send(buzzer_mailbox,1000);
//        x_flag=false;
//        y_flag=false;
//        rt_thread_delay(300);
//        get_location();
//        Car.MileageX=0;
//        Car.MileageY=0;
//        Car.Position_Pointer++;
//    }
}


void reset_mileage(){
    Car.Speed_X=0;
    Car.Speed_Y=0;
    Car.Speed_Z=0;

    Car.MileageX=0;
    Car.MileageY=0;
    Car.DistanceX=0;
    Car.DistanceY=0;
}

//void get_target() {
//    //赋予新的目标坐标点
//    /*
//    nextpoint=locate_route();
//    Car.x1=originMap[nextpoint][0];
//    Car.y1=originMap[nextpoint][1];
//    */

//    if(Car.Position_Pointer<locate_sz){//locate_sz => true_sz改
//        get_route();
//    }
////    if(Car.Position_Pointer<locate_sz){
////        Car.x1=Car_Location_Route[Car.Position_Pointer][0];
////        Car.y1=Car_Location_Route[Car.Position_Pointer][1];
////    }else if(Car.Position_Pointer==locate_sz){
////        Car.x1=0;
////        Car.y1=0;
////    }

//}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     车身定位记录函数
// 参数说明     void
// 返回参数     return_state
// 使用示例     lineate_uart_init();
// 备注信息     \
//-------------------------------------------------------------------------------------------------------------------
void location_place(int x)
{
	
	Car.x = Car.x1;
	Car.x1 = coord[x+1];
	Car.y = Car.y1;
	Car.y1 = coord[x+1];
	
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算车行动路程(测试中)
// 参数说明     void
// 返回参数     return_state
// 使用示例     lineate_uart_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

void move_distance()
{
	 bool x_flag=false,y_flag=false;
	coord[0] = 2;
	coord[1] = 1; 
	
	Car.x1 = coord[0];
	Car.y1 = coord[1];
	
	Car.x = 0;
	Car.y = 0;
	
	Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
	
	Car.DistanceX=20*(Car.x1-Car.x);
	Car.DistanceY=20*(Car.y1-Car.y);
	
	
    if(abs(Car.MileageX)<abs(Car.DistanceX)){
        nextpoint++;
        Car.Speed_X=(speed_tar * sin(Car.Angel_Target/180 *PI));//((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
        if(Car.Speed_X>-1&&Car.Speed_X<0){
            Car.Speed_X=-1;
        }
        if(Car.Speed_X>0&&Car.Speed_X<1){
            Car.Speed_X=1;
        }
        x_flag=false;
    }else{
        Car.Speed_X=0;
        x_flag=true;
    }
    if(abs(Car.MileageY)<abs(Car.DistanceY)){
        Car.Speed_Y=(speed_tar * cos(Car.Angel_Target/180 *PI));
        if(Car.Speed_Y<0&&Car.Speed_Y>-1){
            Car.Speed_Y=-1;
        }
        if(Car.Speed_Y>0&&Car.Speed_Y<1){
            Car.Speed_Y=1;
        }
        y_flag=false;
    }else{
        Car.Speed_Y=0;
        y_flag=true;
    }
	
	
	
	car_omni(Car.Speed_X,Car.Speed_Y,Car.Speed_Z);
	
	motor_run(1);
	
}

