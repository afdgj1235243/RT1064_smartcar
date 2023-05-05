//
// author:czy
//

#ifndef _ATTITUDE_H
#define _ATTITUDE_H


#include "zf_common_headfile.h"

void pid_angel_init();
void attitude_ahead();

//---------------------�ṹ��---------------------//
//��¼��������λ�õĽṹ��
typedef struct location_goal {
    //�ѵ����Ŀ���
    int8_t Position_Pointer;
    //��ǰ�ٶ�
    double Speed_X;
    double Speed_Y;
    double Speed_Z;
    //��ǰλ��
    float x;
    float y;
    //Ŀ��λ��
    float x1;
    float y1;
    //��ǰ��̬
    float Angel;
    //Ŀ����̬
    double Angel_Target;
    //Ŀ�����
    float DistanceX;
    float DistanceY;
    //���ϴ�ת��֮��ǰ���ľ���(��̣�
    float MileageX;
    float MileageY;
}location_goal;



void car_omnimove(void);
void reset_mileage(void);
void keep_Front(void);
void move_distance();

#endif 
