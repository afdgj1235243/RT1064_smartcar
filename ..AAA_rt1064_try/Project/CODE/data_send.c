#include "data_send.h"


uint8 msg[18] = {0};
uint8 encord_mil[18] ={0} ;
    
void send_test()
{
    while (1)
    {
        //按照比赛规则，传入时间，坐标，类别向上位机发送数据
        
        //发送无目标信息
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 1, 2, CATEGORY_NULL);
        //发送正在搬运信息
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 9, 1, CARRY);
				system_delay_ms(2000);
        //发送动物类别信息
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 1, 2, DOG);     
				send_msg(DATA_SEND_UART_INDEX, 123, 456, 1, 2, HORSE);    
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 3, 4, CAT); 
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 5, 6, CATTLE);  
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 7, 8, PIG); 
        system_delay_ms(2000);
        //发送水果类别信息
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 9, 1, ORANGE);  
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 1, 2, APPLE);  
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 3, 4, DURIAN); 
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 5, 6, GRAPE);  
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 7, 8, BANANA);
        system_delay_ms(2000);
        //发送交通工具类别信息
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 9, 1, TRAINS); 
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 1, 2, STEAMSHIP);
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 3, 4, PLANE);
        send_msg(DATA_SEND_UART_INDEX, 123, 456, 5, 6, CAR);
        send_msg(DATA_SEND_UART_INDEX, 234, 567, 7, 8, COACH);
				
        system_delay_ms(2000);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      智能视觉组数据转发模块 发送函数
//  @param      uartn               串口枚举
//  @param      second              运行时长（秒）
//  @param      msecond             运行时长（毫秒）
//  @param      pos_x               X坐标
//  @param      pos_y               Y坐标
//  @param      cg                  类别
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void send_msg(uart_index_enum uartn, uint16 second, uint16 msecond, uint8 pos_x, uint8 pos_y, category_enum cg)
{
    //时间信息
    msg[0] = second/100 + '0';
    msg[1] = second%100/10 + '0';
    msg[2] = second%10 + '0';
    msg[3] = '.';
    msg[4] = msecond/100 + '0';
    msg[5] = msecond%100/10 + '0';
    msg[6] = msecond%10 + '0';
    
    //坐标X信息
    msg[7] = ' ';
    msg[8] = pos_x/10 + '0';
    msg[9] = pos_x%10 + '0';
    
    //坐标Y信息
    msg[10] = ' ';
    msg[11] = pos_y/10 + '0';
    msg[12] = pos_y%10 + '0';
    
    //类别 大类信息
    msg[13] = ' ';
    msg[14] = (uint8)cg/6 + '0';
    
    //类别 小类信息
    msg[15] = ' ';
    msg[16] = (uint8)cg%6 + '0';
    
    msg[17] = '\n';
    uart_write_buffer(uartn, msg, 18);
}

void send_encord(uart_index_enum uartn, uint8 encord_addition)
{
	
	
	encord_mil[0] = encord_addition + '0';
	encord_mil[1] = encord_addition + '0';
	encord_mil[2] = encord_addition + '0';
	encord_mil[3] = encord_addition + '0';
	encord_mil[4] = encord_addition + '0';
	encord_mil[5] = encord_addition + '0';
	encord_mil[6] = encord_addition + '0';
	encord_mil[7] = encord_addition + '0';
	encord_mil[8] = encord_addition + '0';
	encord_mil[9] = encord_addition + '0';
	encord_mil[10] = encord_addition + '0';
	encord_mil[11] = encord_addition + '0';
	encord_mil[12] = encord_addition + '0';
	encord_mil[13] = encord_addition + '0';
	encord_mil[14] = encord_addition + '0';
	encord_mil[15] = encord_addition + '0';
	encord_mil[16] = encord_addition + '0';
	encord_mil[17] = '\n';
	
	uart_write_buffer(uartn, encord_mil, 18);
}


void data_send_uart_init()
{
	uart_init(DATA_SEND_UART_INDEX,115200,DATA_SEND_UART_INDEX_TX,DATA_SEND_UART_INDEX_RX);
	
}