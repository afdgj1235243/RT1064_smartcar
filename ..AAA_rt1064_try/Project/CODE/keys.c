/*����
�������ڣ�2023/4/18
byС��Ź���*/
#include "keys.h"


void my_keyInit(void)//������ʼ��
{
    gpio_init(C12,GPI, GPIO_HIGH, GPI_PULL_UP);//ȷ�ϼ�
    gpio_init(C13,GPI, GPIO_HIGH, GPI_PULL_UP);//���ؼ�
    gpio_init(C14,GPI, GPIO_HIGH, GPI_PULL_UP);//���Ƽ����Ӽ���
    gpio_init(C15,GPI, GPIO_HIGH, GPI_PULL_UP);//���Ƽ���������
}
/**
  * @brief  ���̶�ȡ��������
  * @param  ��
  * @retval KeyNumber ���°����ļ���ֵ
            ����������²��ţ������ͣ���ڴ˺��������ֵ�һ˲�䣬���ذ������룬û�а�������ʱ������0
  */
unsigned char MatrixKey(void)
{
    unsigned char KeyNumber=4;

//    if(gpio_get_level(P20_6)==0){system_delay_ms(100);if(gpio_get_level(P20_9)==0);system_delay_ms(20);KeyNumber=4;}
//    if(gpio_get_level(P20_7)==0){system_delay_ms(100);if(gpio_get_level(P20_9)==0);system_delay_ms(20);KeyNumber=5;}
//    if(gpio_get_level(P20_8)==0){system_delay_ms(100);if(gpio_get_level(P20_9)==0);system_delay_ms(20);KeyNumber=6;}
//    if(gpio_get_level(P20_9)==0){system_delay_ms(100);if(gpio_get_level(P20_9)==0);system_delay_ms(20);KeyNumber=7;}

    if(gpio_get_level(C12)==0){system_delay_ms(20);while(gpio_get_level(C12)==0);
		system_delay_ms(20);KeyNumber=0;}
    if(gpio_get_level(C13)==0){system_delay_ms(20);while(gpio_get_level(C13)==0);
		system_delay_ms(20);KeyNumber=1;}
    if(gpio_get_level(C14)==0){system_delay_ms(20);while(gpio_get_level(C14)==0);
		system_delay_ms(20);KeyNumber=2;}
    if(gpio_get_level(C15)==0){system_delay_ms(20);while(gpio_get_level(C15)==0);
		system_delay_ms(20);KeyNumber=3;}



    return KeyNumber;
}



