/*按键
创建日期：2023/4/18
by小天才狗蛋*/
#include "keys.h"


void my_keyInit(void)//按键初始化
{
    gpio_init(C12,GPI, GPIO_HIGH, GPI_PULL_UP);//确认键
    gpio_init(C13,GPI, GPIO_HIGH, GPI_PULL_UP);//返回键
    gpio_init(C14,GPI, GPIO_HIGH, GPI_PULL_UP);//右移键（加键）
    gpio_init(C15,GPI, GPIO_HIGH, GPI_PULL_UP);//左移键（减键）
}
/**
  * @brief  键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
            如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
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



