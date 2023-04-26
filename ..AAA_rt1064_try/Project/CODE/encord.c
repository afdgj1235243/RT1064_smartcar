#include "encord.h"



int32 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//��������ֵ



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
	
	
    //����λ��(��λ��m)
    //Car.mileage=(Encoder/1024)*(45/104)*2*PI*0.03;


	
	

    encoder_clear_count(QTIMER1_ENCODER1 );
    encoder_clear_count(QTIMER1_ENCODER2 );
    encoder_clear_count(QTIMER2_ENCODER1 );
    encoder_clear_count(QTIMER2_ENCODER2 );
}