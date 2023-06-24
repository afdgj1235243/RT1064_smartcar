

#ifndef _DATA_SEND_H
#define _DATA_SEND_H

#include "zf_common_headfile.h"
#define DATA_SEND_UART_INDEX    (UART_1)
#define DATA_SEND_UART_INDEX_TX  (UART1_TX_B12)
#define DATA_SEND_UART_INDEX_RX  (UART1_RX_B13)

typedef enum
{
    CATEGORY_NULL   = 0,        //无类别
    DOG             = 7,        //狗
    HORSE           = 8,        //马
    CAT             = 9,        //猫
    CATTLE          = 10,       //牛
    PIG             = 11,       //猪
    ORANGE          = 13,       //橙子
    APPLE           = 14,       //苹果
    DURIAN          = 15,       //榴莲
    GRAPE           = 16,       //葡萄
    BANANA          = 17,       //香蕉
    TRAINS          = 19,       //火车
    STEAMSHIP       = 20,       //轮船
    PLANE           = 21,       //飞机
    CAR             = 22,       //小轿车
    COACH           = 23,       //大客车
    CARRY           = 54,       //搬运
}category_enum;


void send_msg(uart_index_enum uartn, uint16 second, uint16 msecond, uint8 pos_x, uint8 pos_y, category_enum cg);

void data_send_uart_init();

void send_test();

void send_encord(uart_index_enum uartn, uint8 encord_addition);
#endif 
