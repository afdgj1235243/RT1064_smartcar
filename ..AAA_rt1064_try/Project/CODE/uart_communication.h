//
// author:czy
//


#ifndef _UART_COMMUNICATION_H
#define _UART_COMMUNICATION_H

#include "zf_common_headfile.h"

#define UART_DATA_BUFF  (500)  //�����������ֽ���
#define UART_BUFF_OK  1
#define UART_BUFF_ERROR  0

#define LINEATE_UART_INDEX         (UART_4)                                    // openart���ڶ�Ӧʹ�õĴ��ں�    ��ǰΪuart4�����ţ����ڿ��������
#define LINEATE_UART_BUAD_RATE     (115200)                                    // openart���ڶ�Ӧʹ�õĴ��ڲ�����
#define LINEATE_UART_TX_PIN        (UART4_RX_C17)                              // openart���ڶ�Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX(UART4_RX_C17) 
#define LINEATE_UART_RX_PIN        (UART4_TX_C16)                              // openart���ڶ�Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define LINEATE_UART_RTS_PIN       (D26)                                       // openart���ڶ�Ӧģ��� RTS ����

#define LINEATE_UART_AUTO_BAUD_RATE    (0)//�ر��Զ������ʣ��Զ������ʵ��������޸� LINEATE_UART_BAUD ֮����Ҫ��ģ��������� ģ����Զ�����Ϊ��Ӧ�Ĳ�����


#define LINEATE_UART_BUFFER_SIZE       (64 )   //Ĭ�ϻ�������64�ֽ�
#define LINEATE_UART_TIMEOUT_COUNT     (100)   //



typedef struct
{
	uint16 head;
	uint16 end;
	uint16 len;
	

}lineate_buff;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint8_t add[100];
	float k;

}location_now;

typedef struct
{
	uint8 map_data;
	uint8 X[150];
	uint8 Y[150];
	uint8 add[150];
	uint16 len;

}jieshou_try_need;




void lineate_uart_callback (void);

void lineate_uart_try(void);

uint8 lineate_uart_init(void);

uint32 lineate_uart_buff_read(uint8 *buff, uint32 len);


uint32 lineate_uart_buff_send (const uint8 data);
uint32 lineate_uart_send_byte (const uint8 data);
	
void set_lineate_type (wireless_type_enum type_set, callback_function wireless_callback);

void jieshou_map(int8 x);

void jieshou_try(int8 x);

void fifo_text();

void location_point_read();
uint8_t location_correct_point_read();

#endif 
