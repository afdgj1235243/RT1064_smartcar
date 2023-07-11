#ifndef _smotor_h
#define _smotor_h

#include "zf_common_headfile.h"

void smotor_init(void);
void smotor_updown_control(uint32 duty);
	
void grab_picture();
void place_picture();

void smotor_turn_control(int16 duty);
void smotor3_control(int16 duty);

int smotor_pid(int now_position,int Target);

void stepping_motor_control(uint16 run);

void picture_read_countrol();

uint16 step_command(uint8 step);
#endif
