//
// author:czy
//

#ifndef _ENCORD_H
#define _ENCORD_H

#include "zf_driver_encoder.h"
#include "zf_common_headfile.h"



void encord_init();
void encoder_get(void);
void omni_mileage();
void encoderjiajia();

struct RC_Para
{
    float temp;  //暂存值,存储RC_baro
    float value; //滤波值
    float RC;    //低通滤波参数
};
typedef struct RC_Para *RC_Filter_pt;

float RCFilter(float value,RC_Filter_pt Filter);

void encoder_init();

#endif 
