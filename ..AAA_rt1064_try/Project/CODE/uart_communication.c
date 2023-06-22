//uart串口通信
//
//author：czy
//
//最后修改时间：2023.4.2


#include "uart_communication.h"
//#define LED1                    (B9 )  //测试设置，不用try时注释

uint8 							lineate_uart_rx_buffer[LINEATE_UART_BUFFER_SIZE];  //定义接收缓冲区，当前直接使用默认缓冲区长度
uint8 							lineate_uart_rx_buffer_len;  //接收缓冲区长度


//uint8              lineate_uart_tx_buffer;  //发送缓冲区，少部分发送可以先不用缓冲区
//uint8 							lineate_uart_rx_buffer_len;  //发送缓冲区长度，少部分发送可以先不用缓冲区


static fifo_struct lineate_uart_fifo;  //定义一个FIFO对象指针，即为我的user_fifo，在接下来FIFO 初始化 挂载对应缓冲区中使用（fifo_init）

static          uint8       lineate_uart_data;  //回调函数中读到的数据存放，再放入缓冲区时用

 jieshou_try_need jieshoushuju;

//uint8 data_buffer[32];  //测试设置，不用try时注释
//uint8 data_len;  //测试设置，不用try时注释

uint8               openart_rx_buffer;
lpuart_transfer_t   openart_receivexfer;
lpuart_handle_t     openart_g_lpuartHandle;

uint8               openart1_rx_buffer;
lpuart_transfer_t   openart1_receivexfer;
lpuart_handle_t     openart1_g_lpuartHandle;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     用于无模式的uart串口输出，测试使用
// 参数说明     uint8
// 返回参数     return_state
// 使用示例     lineate_uart_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

uint8 lineate_uart_init(void)
{
	uint8 return_state = 0;
	
	set_lineate_type(LINEATE, lineate_uart_callback);   

	//FIFO配置（接收可加）发送缓冲区

	fifo_init(&lineate_uart_fifo,FIFO_DATA_8BIT,lineate_uart_rx_buffer, LINEATE_UART_BUFFER_SIZE);    //初始化FIFO缓冲区，FIFO 对象指针为lineate_uart_fifo，数据位数为8比特，挂载的缓冲区为lineate_uart_rx_buffer，其长度LINEATE_UART_BUFFER_SIZE可在.h文件中更改
	
//  gpio_init(LINEATE_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);              //初始化gpio输出，引脚为LINEATE_UART_RTS_PIN，默认高电平 推挽输出模式
#if(0 == LINEATE_UART_AUTO_BAUD_RATE)                                          // 关闭自动波特率
    // 本函数使用的波特率为115200 为无线转串口模块的默认波特率 如需其他波特率请自行配置模块并修改串口的波特率
		uart_init(LINEATE_UART_INDEX, LINEATE_UART_BUAD_RATE, LINEATE_UART_RX_PIN, LINEATE_UART_TX_PIN);  //后期看引脚记得改一下，初始化uart接收与发送引脚
		
		tft180_show_string(0,60,"error");
	
		NVIC_SetPriority((IRQn_Type)((IRQn_Type)(LINEATE_UART_INDEX) + LPUART1_IRQn),0); 
	
    uart_rx_interrupt(LINEATE_UART_INDEX, 1);					// 打开串口1接收完成中断
	
#endif
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取串口接收缓冲区
// 参数说明     uint32 
// 返回参数     data_len
// 使用示例     lineate_uart_try();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

uint32 lineate_uart_buff_read(uint8 *buff, uint32 len)
{
	zf_assert(buff != NULL);
    uint32 data_len = len;
	
    fifo_read_buffer(&lineate_uart_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);  //模式为FIFO_READ_AND_CLEAN，读取后清除缓冲区被读取的数据，buff为目标缓冲区
	
    return data_len;
	
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     发送串口发送字节
// 参数说明     uint32 
// 返回参数     data_len
// 使用示例     lineate_uart_try();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------


uint32 lineate_uart_send_byte (const uint8 data)
{
    uint16 time_count = LINEATE_UART_TIMEOUT_COUNT;
	
    while(time_count)
    {
        if(!gpio_get_level(LINEATE_UART_TIMEOUT_COUNT))
        {
            uart_write_byte(LINEATE_UART_TIMEOUT_COUNT, data);                         
            break;
        }
        time_count --;
        system_delay_ms(1);
    }
    return (0 < time_count);
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     测试函数，检测基本接收功能是否可以使用
// 参数说明     void
// 返回参数     void
// 使用示例     lineate_uart_try();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

void lineate_uart_try(void)
{
    uint8 count = 0;

//    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
	
    if(lineate_uart_init())                                                    // 判断是否通过初始化
    {
        while(1)                                                                // 初始化失败就在这进入死循环
        {
//            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
					tft180_show_string(0,60,"lineate_uart_init error");
          system_delay_ms(100);                                               // 显示异常
        }
    }
		while(1){
			tft180_show_string(0,60,"lineate_uart_init");
//			lineate_uart_send_byte('\r');
			uart_write_byte(UART_4,1);
			
		}
//    lineate_uart_send_byte('\n');
//    lineate_uart_send_string("SEEKFREE wireless uart demo.\r\n");              // 初始化正常 输出测试信息
    
//    interrupt_global_enable(0);
    
    // 此处编写用户代码 例如外设初始化代码等

//    while(1)
//    {
//        // 此处编写需要循环执行的代码
//        data_len = wireless_uart_read_buff(data_buffer, 32);                    // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
//        if(data_len != 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
//        {
//            wireless_uart_send_buff(data_buffer, data_len);                     // 将收到的消息发送回去
//            memset(data_buffer, 0, 32);
//            func_uint_to_str((char *)data_buffer, data_len);
//            wireless_uart_send_string("\r\ndata len:");                                 // 显示实际收到的数据信息
//            wireless_uart_send_buff(data_buffer, strlen((const char *)data_buffer));    // 显示收到的数据个数
//            wireless_uart_send_string(".\r\n");
//        }
////        if(count ++ > 10)
////        {
////            count = 0;
////            gpio_toggle_level(LED1);                                            // 翻转 LED 引脚输出电平 控制 LED 亮灭
////        }
//        system_delay_ms(50);
//    }


	}		
//		
	
	


		
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无模式串口回调函数
// 参数说明     uint32 
// 返回参数     data_len
// 使用示例     lineate_uart_try();
// 备注信息     初步回调函数，目前处于测试使用，后期根据要发送的数据更改代码功能
//-------------------------------------------------------------------------------------------------------------------
	
void lineate_uart_callback (void)
{
	uart_query_byte(LINEATE_UART_INDEX, &lineate_uart_data);
	fifo_write_buffer(&lineate_uart_fifo, &lineate_uart_data, 1);
	
#if WIRELESS_UART_AUTO_BAUD_RATE                                                // 开启自动波特率
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8 *)wireless_auto_baud_data, (uint32 *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif


}





//void jieshou_try(int8 x)
//{
//	uint8 i=0;
//	while(1)
//{
//	
//	jieshoushuju.map_data = uart_read_byte(UART_1);

////if(jieshoushuju.map_data!=jieshoushuju.add[i-1])
////{ 
//	jieshoushuju.add[i]=jieshoushuju.map_data - 48;
//	i++;
////	}
//if(i==x+1)
//{	i=1;
//	jieshoushuju.map_data=0;
//	break;}
//}
//}




//临时通信用代码，lens为自己确定的坐标个数
//void jieshou_map(int8 lens)
//{
//	uint16 time_count = LINEATE_UART_TIMEOUT_COUNT;
//	uint8 i =1;
//		while(1){
//		if(gpio_get_level(C12)==0){
//			
//			tft180_clear();
//			tft180_show_string(0,0,"uart_out");
//			while(gpio_get_level(C12)==0);
//			system_delay_ms(20);
//			tft180_clear();
//			uart_write_string(UART_4, "2\n");
//			break;
//		}
//	}
//		
//		for(int i = 1;i<lens;i++)
//		{
//			jieshoushuju.map_data = uart_read_byte(UART_4);
//			if(i%2==1){
//			jieshoushuju.X[i] = jieshoushuju.map_data -48;
//			}else{
//			jieshoushuju.Y[i] = jieshoushuju.map_data -48;
//			}
//		}			
//	
//	while(1)
//	{
//	jieshoushuju.map_data = uart_read_byte(UART_4);
//		jieshoushuju.add[i] = jieshoushuju.map_data;
//		i++;
//		if(i>lens)
//			break;
//	}
	
	
	
//}

void fifo_text(uint8 *buff)
{
	int i;
	
//	uint32 len = lineate_uart_rx_buffer_len;
//	zf_log(fifo_init(&lineate_uart_fifo,FIFO_DATA_8BIT, lineate_uart_rx_buffer, 64) == FIFO_SUCCESS, "fifo_read_buffer error");
	
	i =lineate_uart_buff_read(lineate_uart_rx_buffer,4);
//	tft180_show_uint(0,0,i,3);
	tft180_show_uint(0,2*16,lineate_uart_rx_buffer[0],3);
	tft180_show_uint(20,2*16,lineate_uart_rx_buffer[1],3);
	tft180_show_uint(40,2*16,lineate_uart_rx_buffer[2],3);
	tft180_show_uint(60,2*16,lineate_uart_rx_buffer[3],3);

}

