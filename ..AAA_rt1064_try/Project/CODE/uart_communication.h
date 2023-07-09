//
// author:czy
//


#ifndef _UART_COMMUNICATION_H
#define _UART_COMMUNICATION_H

#include "zf_common_headfile.h"

#define UART_DATA_BUFF  (500)  //定义最大接收字节数
#define UART_BUFF_OK  1
#define UART_BUFF_ERROR  0

#define LINEATE_UART_INDEX         (UART_4)                                    // openart串口对应使用的串口号    当前为uart4号引脚，后期看情况更改
#define LINEATE_UART_BUAD_RATE     (115200)                                    // openart串口对应使用的串口波特率
#define LINEATE_UART_TX_PIN        (UART4_RX_C17)                              // openart串口对应模块的 TX 要接到单片机的 RX(UART4_RX_C17) 
#define LINEATE_UART_RX_PIN        (UART4_TX_C16)                              // openart串口对应模块的 RX 要接到单片机的 TX
#define LINEATE_UART_RTS_PIN       (D26)                                       // openart串口对应模块的 RTS 引脚

#define LINEATE_UART_AUTO_BAUD_RATE    (0)//关闭自动波特率，自动波特率的作用是修改 LINEATE_UART_BAUD 之后不需要对模块进行配置 模块会自动设置为对应的波特率


#define LINEATE_UART_BUFFER_SIZE       (64 )   //默认缓冲区，64字节
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
