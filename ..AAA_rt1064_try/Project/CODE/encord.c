#include "encord.h"

extern location_goal Car;

int32 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//编码器的值
int32 RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4;
float MileageKx=0.01079f;
float MileageKy=0.00999f;
//float MileageKx=0.0001f;
//float MileageKy=0.0001f;

//float MileageKx_my=0.007943;
//float MileageKy_my=0.007943;
//float MileageKx_my=0.00789;
//float MileageKy_my=0.00789;
float MileageKx_my=0.00820;
float MileageKy_my=0.00839;

struct RC_Para Encoder1_Para = {0,0,0.25};
struct RC_Para Encoder2_Para = {0,0,0.25};
struct RC_Para Encoder3_Para = {0,0,0.25};
struct RC_Para Encoder4_Para = {0,0,0.25};

RC_Filter_pt RC_Encoder1 = &Encoder1_Para;
RC_Filter_pt RC_Encoder2 = &Encoder2_Para;
RC_Filter_pt RC_Encoder3 = &Encoder3_Para;
RC_Filter_pt RC_Encoder4 = &Encoder4_Para;



//typedef enum
//{
//    QTIMER1_ENCODER1 = 2,
//    QTIMER1_ENCODER2,
//    
//    QTIMER2_ENCODER1,
//    QTIMER2_ENCODER2,
//    
//    QTIMER3_ENCODER1,
//    QTIMER3_ENCODER2,
//    
//    QTIMER4_ENCODER1,
//    QTIMER4_ENCODER2,
//}encoder_index_enum;




void encord_init()
{
	encoder_dir_init(QTIMER1_ENCODER1, QTIMER1_ENCODER1_CH1_C0, QTIMER1_ENCODER1_CH2_C1);
	
	encoder_dir_init(QTIMER1_ENCODER2, QTIMER1_ENCODER2_CH1_C2, QTIMER1_ENCODER2_CH2_C24);
	
	encoder_dir_init(QTIMER2_ENCODER1, QTIMER2_ENCODER1_CH1_C3, QTIMER2_ENCODER1_CH2_C4);
	
	encoder_dir_init(QTIMER2_ENCODER2, QTIMER2_ENCODER2_CH1_C5, QTIMER2_ENCODER2_CH2_C25);

}



void encoder_get(void)
{
    encoder3 = encoder_get_count(QTIMER1_ENCODER1 ); 
    encoder4 = encoder_get_count(QTIMER1_ENCODER2); 
    encoder2 = -encoder_get_count(QTIMER2_ENCODER1); 
    encoder1 = -encoder_get_count(QTIMER2_ENCODER2);
	
	
    //计算位移(单位：m)
    //Car.mileage=(Encoder/1024)*(45/104)*2*PI*0.03;
    //计算速度（单位：m/s）
		////V = N/1024*（30/68*20.1/100*20） = N*0.0017 m/s
		//求出车速转换为M/S  N/1024 *（30/68*20.1cm/50ms） = Vm/s,30为编码器齿轮，68为传动齿轮，20.1为轮子周长，控制周期50ms
		
		
    RC_encoder1 = (int16_t)RCFilter(encoder1,RC_Encoder1);
    RC_encoder2 = (int16_t)RCFilter(encoder2,RC_Encoder2);
    RC_encoder3 = (int16_t)RCFilter(encoder3,RC_Encoder3);
    RC_encoder4 = (int16_t)RCFilter(encoder4,RC_Encoder4);

		omni_mileage();
		encoderjiajia();

    encoder_clear_count(QTIMER1_ENCODER1 );
    encoder_clear_count(QTIMER1_ENCODER2 );
    encoder_clear_count(QTIMER2_ENCODER1 );
    encoder_clear_count(QTIMER2_ENCODER2 );
}


float RCFilter(float value,RC_Filter_pt Filter)
{
    Filter->temp = value;
    Filter->value = (1 - Filter->RC) * Filter->value + Filter->RC * Filter->temp;
//	temp = RC * value + (1 - RC) * temp;
    return Filter->value;
}

float detaxx,detayy;

void omni_mileage(){
    float detax=0,detay=0;
    detax=(float)(RC_encoder1 - (-RC_encoder2) + RC_encoder4 - (-RC_encoder3))/4;
    detay=(float)(RC_encoder1 + (-RC_encoder2) + (-RC_encoder3) + RC_encoder4)/4;
	
//	  detax=(float)(RC_encoder1 - RC_encoder2 + RC_encoder4 - RC_encoder3)/4;
//    detay=(float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4)/4;

//    detax=(float)(((RC_encoder1 - (-RC_encoder2) + RC_encoder4 - (-RC_encoder3))/4)/1024*(45/104*18.2));
//    detay=(float)((RC_encoder1 + (-RC_encoder2) + (-RC_encoder3) + RC_encoder4)/4)/1024*(45/104*18.2);
		
    Car.MileageX+=(float)(detax*MileageKx_my);
    Car.MileageY+=(float)(detay*MileageKy_my);
	
		detaxx=detax,detayy=detay;
}

void encoderjiajia()
{
		Car.encord_add1+=(RC_encoder1*MileageKx)*0.8;
		Car.encord_add2+=(RC_encoder2*MileageKy)*0.8;
		Car.encord_add3+=(RC_encoder3*MileageKy)*0.8;
		Car.encord_add4+=(RC_encoder4*MileageKx)*0.8;
	
}


void encoder_init()
{
	Car.encord_add1=0;
	Car.encord_add2=0;
	Car.encord_add3=0;
	Car.encord_add4=0;
	
	 Car.MileageX = 0;
	 Car.MileageY = 0;
	

}	
