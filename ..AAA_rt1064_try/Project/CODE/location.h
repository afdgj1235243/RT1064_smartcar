

#ifndef _LOCATION_H
#define _LOCATION_H

#include "zf_common_headfile.h"


void car_locationread();
void move_test();
void location_lines(int8 x);
void main_movement(int point);
void location_shortest();
void location_swtich(int x);
void image_find_move();


bool boold_judue(int8 image_judge_point_x,int8 image_judge_point_y);      //色块判断函数

int16 location_correct_pid_x(int8 locaton_target_x,int8 now_location_x);  //定位PID函数
int16 location_correct_pid_y(int8 locaton_target_y,int8 now_location_y);

#endif 
