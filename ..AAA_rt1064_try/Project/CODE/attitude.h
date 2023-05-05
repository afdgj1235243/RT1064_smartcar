//
// author:czy
//

#ifndef _ATTITUDE_H
#define _ATTITUDE_H


#include "zf_common_headfile.h"

void pid_angel_init();
void attitude_ahead();

//---------------------结构体---------------------//
//记录坐标和相对位置的结构体
typedef struct location_goal {
    //已到达的目标点
    int8_t Position_Pointer;
    //当前速度
    double Speed_X;
    double Speed_Y;
    double Speed_Z;
    //当前位置
    float x;
    float y;
    //目标位置
    float x1;
    float y1;
    //当前姿态
    float Angel;
    //目标姿态
    double Angel_Target;
    //目标距离
    float DistanceX;
    float DistanceY;
    //距上次转向之后前进的距离(里程）
    float MileageX;
    float MileageY;
}location_goal;



void car_omnimove(void);
void reset_mileage(void);
void keep_Front(void);
void move_distance();

#endif 
