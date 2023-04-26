//
// author:czy
//

#ifndef _ICM20602_H
#define _ICM20602_H


#include "zf_common_headfile.h"

extern volatile float twoKp;          // 2 * proportional gain (Kp)
extern volatile float twoKi;          // 2 * integral gain (Ki)
extern volatile float q0, q1, q2, q3; // quaternion of sensor frame relative to auxiliary frame

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
    float mag_x;
    float mag_y;
    float mag_z;
} arhs_source_param_t;

typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} mag_zero_param_t;

typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_zero_param_t;

typedef struct {
    float pitch;    //¸©Ñö½Ç
    float roll;     //Æ«º½½Ç
    float yaw;       //·­¹ö½Ç
} arhs_euler_param_t;

extern arhs_euler_param_t arhs_data;




/***************************µÚ¶þÖÖ´úÂë**************************************/
typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} icm_param_t;


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quater_param_t;


typedef struct {
    float pitch;    //¸©Ñö½Ç
    float roll;     //Æ«º½½Ç
    float yaw;       //·­¹ö½Ç
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern quater_param_t Q_info ;  // È«¾ÖËÄÔªÊý
extern euler_param_t eulerAngle;

void gyroOffset_init(void);

float fast_sqrt(float x);

void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);

void ICM_getValues();

void ICM_getEulerianAngles(void);

//---------------------------------------------------------------------------------------------------
// Function declarations

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void Mahony_computeAngles(void);
void imu_offset_init(void);





#endif 
