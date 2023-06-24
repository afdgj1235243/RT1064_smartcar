

#ifndef _DATA_SEND_H
#define _DATA_SEND_H

#include "zf_common_headfile.h"
#define DATA_SEND_UART_INDEX    (UART_1)
#define DATA_SEND_UART_INDEX_TX  (UART1_TX_B12)
#define DATA_SEND_UART_INDEX_RX  (UART1_RX_B13)

typedef enum
{
    CATEGORY_NULL   = 0,        //�����
    DOG             = 7,        //��
    HORSE           = 8,        //��
    CAT             = 9,        //è
    CATTLE          = 10,       //ţ
    PIG             = 11,       //��
    ORANGE          = 13,       //����
    APPLE           = 14,       //ƻ��
    DURIAN          = 15,       //����
    GRAPE           = 16,       //����
    BANANA          = 17,       //�㽶
    TRAINS          = 19,       //��
    STEAMSHIP       = 20,       //�ִ�
    PLANE           = 21,       //�ɻ�
    CAR             = 22,       //С�γ�
    COACH           = 23,       //��ͳ�
    CARRY           = 54,       //����
}category_enum;


void send_msg(uart_index_enum uartn, uint16 second, uint16 msecond, uint8 pos_x, uint8 pos_y, category_enum cg);

void data_send_uart_init();

void send_test();

void send_encord(uart_index_enum uartn, uint8 encord_addition);
#endif 
