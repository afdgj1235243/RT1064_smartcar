//
// author:czy
//

#ifndef _MOVE_H
#define _MOVE_H
#include "zf_common_headfile.h"
#include "encord.h"
extern int32 encoder1,encoder2,encoder3,encoder4;


void motor_init(void);


void car_ahead();
void car_back();
void car_turnleft();
void car_turnright();
void car_diagonal();
void car_turnround();
void car_anticlockwise();
void car_concerning();
void car_stop();


void car_ahead_new();
void car_back_new();
void car_turnleft_new();
void car_turnright_new();
void car_diagonal_new();
void car_turnround_new();
void car_anticlockwise_new();
void car_concerning_new();
void car_stop_new();
void car_omni(float x, float y, float z);;

void pid_calculate(void);
void motor_run(bool run);
void motor_control(bool run);
int limit_pid_pwm(int32 duty_in);
void try();


int position_pid(int Encoder,int Target);


#endif 