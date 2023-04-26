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


//bool data_head = 0;
//bool data_last = 0;

 


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     仿制无线串口写的初始化，用于无模式的uart串口输出，测试使用
// 参数说明     uint8
// 返回参数     return_state
// 使用示例     lineate_uart_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------

uint8 lineate_uart_init(void)
{
	uint8 return_state = 0;
	
	set_wireless_type(LINEATE, lineate_uart_callback);   //这里的中断回调函数复用了wireless_uart_callback，测试时注意！！！, 同时注意设置模块类型时选择了无模块，不知道会不会出问题，如有问题，重点复查这里
	//(这里很大可能得自己写一个lineate的模式回调函数，因为单纯调成NO_WIRELESS似乎一定会进入type_default_callback，返回的是void)
	
	
	//FIFO配置（接收可加）发送缓冲区
	
	fifo_init(&lineate_uart_fifo,FIFO_DATA_8BIT,lineate_uart_rx_buffer, LINEATE_UART_BUFFER_SIZE);    //初始化FIFO缓冲区，FIFO 对象指针为lineate_uart_fifo，数据位数为8比特，挂载的缓冲区为lineate_uart_rx_buffer，其长度LINEATE_UART_BUFFER_SIZE可在.h文件中更改
  gpio_init(LINEATE_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);              //初始化gpio输出，引脚为LINEATE_UART_RTS_PIN，默认高电平 推挽输出模式
#if(0 == LINEATE_UART_AUTO_BAUD_RATE)                                          // 关闭自动波特率
    // 本函数使用的波特率为115200 为无线转串口模块的默认波特率 如需其他波特率请自行配置模块并修改串口的波特率
		uart_init(LINEATE_UART_INDEX, LINEATE_UART_BUAD_RATE, LINEATE_UART_RX_PIN, LINEATE_UART_TX_PIN);  //后期看引脚记得改一下，初始化uart接收与发送引脚
	
//    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // 初始化串口
    uart_rx_interrupt(LINEATE_UART_INDEX, 1);					// 打开串口1接收完成中断
#elif(1 == LINEATE_UART_AUTO_BAUD_RATE)                                        // 开启自动波特率
    uint8 rts_init_status;
    uint16 time_count = 0;

    wireless_auto_baud_flag = 0;
    wireless_auto_baud_data[0] = 0;
    wireless_auto_baud_data[1] = 1;
    wireless_auto_baud_data[2] = 3;

    rts_init_status = gpio_get_level(WIRELESS_UART_RTS_PIN);
    gpio_init(WIRELESS_UART_RTS_PIN, GPO, rts_init_status, GPO_PUSH_PULL);      // 初始化流控引脚

    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // 初始化串口
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);

    system_delay_ms(5);                                                         // 模块上电之后需要延时等待
    gpio_set_level(WIRELESS_UART_RTS_PIN, !rts_init_status);                    // RTS引脚拉高，进入自动波特率模式
    system_delay_ms(100);                                                       // RTS拉高之后必须延时20ms
    gpio_toggle(WIRELESS_UART_RTS_PIN);                                         // RTS引脚取反

    wireless_auto_baud_flag = 1;

    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[0]);           // 发送特定数据 用于模块自动判断波特率
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[1]);           // 发送特定数据 用于模块自动判断波特率
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[2]);           // 发送特定数据 用于模块自动判断波特率
    system_delay_ms(20);

    time_count = 0;
    do
    {
        if(3 != wireless_auto_baud_flag)                                        // 检验自动波特率是否完成
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // 如果程序进入到此语句内 说明自动波特率失败了
            break;
        }

        time_count = 0;
        if( 0xa5 != wireless_auto_baud_data[0] &&                               // 检验自动波特率是否正确
            0xff != wireless_auto_baud_data[1] &&                               // 检验自动波特率是否正确
            0xff != wireless_auto_baud_data[2] )                                // 检验自动波特率是否正确
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // 如果程序进入到此语句内 说明自动波特率失败了
            break;
        }
        wireless_auto_baud_flag = 0;

        gpio_init(WIRELESS_UART_RTS_PIN, GPI, 0, GPI_PULL_UP);                  // 初始化流控引脚
        system_delay_ms(10);                                                    // 延时等待 模块准备就绪
    }while(0);
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

//-------------------------------------------------------------------------------------------------------------------

	
//说明：以下代码为参考程序之一，来源于CSDN，谨慎参考	
	
	
//-------------------------------------------------------------------------------------------------------------------
	
	
////UART波特率是9600,1 bit≈104us，
////1个起始位，1个停止位，8个数据位
////每个数据包帧头是0xAA，帧尾0x0D，校验值是除了帧尾所有数据的累加和。
////通讯方式采用一问一答形式，每500ms发送一次数据并接收应答数据，
////接收数据总共22byte，22*(1+1+8)*104us=22.88ms,即接收每帧数据的时间长度.
// 
//#define d_head  0xAA
//#define d_tail  0x0D
// 
//volatile bit RXFLAG = 0;//外部中断标志位,在下降沿中断中置1;
//u8	Rx_Tim=0;    //接收计时器，每次接收到数据时重置倒计时。倒计时为0则认为上一帧结束,        
//                 //开始接收下一帧数据，复位接收计数器。
//#define Set_Clear_RxTim()        Rx_Tim=50    //50ms倒计时，我的产品接收数据时间长度22.88ms
//                                              //理论上23ms就可以了，不过要考虑时序存在误差。
//                
//void Uart_IO_GPIO_Init(void)//模拟串口的IO口初始化
//{
//    PORTA|= 0B00000001;		//Tx输出1
//    TRISA &= 0B11111110;    //PA0-Output_Tx  PA1-Input_Rx
//    WPUA  |= 0B00000010;    //PA1上拉电阻
//}
// 
//void WaitTF0(void)//延时等待控制UART时序
//{
//     while(T2UIE);//定时器开始计时后，在定时器中断中清零T2UIE
//     T2UIE=1;
//}
// 
//void WByte(u8 input)//发送1 Byte
//{
//	u8 i=8;
//	TXIO=0;     //Tx置0，发送起始位
//    T2UIF = 1;  //清零定时器2中断标志位
//	T2CEN =0;   //失能定时器2
//	TIM2CNTRH=0;//清零定时器2计数值高位
//	TIM2CNTRL=0;//清零定时器2计数值低位
//	T2CEN=1;    //启动定时器2
//	T2UIE =1;   //使能定时器2
//	WaitTF0();  //等待定时器中断，等待时间取决于定时器2的装载值。这里的定时时间是104us
//	while(i--)
//	{
//		if(input&0x01) //逐个bit发送，先传低位
//			TXIO=1;
//		else
//			TXIO = 0;   
//		WaitTF0(); //等待延时104us
//		input=input>>1; 
//	}
//	TXIO=1;      //发送结束位
//	WaitTF0();
//	T2UIE=0;     //发送1 byte完成了     
//}
// 
// 
//u8 RByte()//接收1 Byte
//{
//	u8 Output=0;
//	u8 i=8;
//    T2UIF = 1;    //清零定时器2中断标志位
//    T2CEN=0;      //失能定时器2
//	TIM2CNTRH=0;  //清零定时器2计数值高位
//	TIM2CNTRL=0;  //清零定时器2计数值低位
//    TIM2ARRH =0x09; //设定时器2装载值
//	TIM2ARRL =0x00;	//起始位1.5bit(156us)后再读取电平，确保在第一bit中间位置读取数据
//    T2CEN=1;        //启动定时器2                
//    T2UIE =1;       //使能定时器2
//	WaitTF0();      //等待104us+52us=156us，刚好在第一个bit的中间位置读取数据
//    T2CEN=0;        //失能定时器2
//  	TIM2ARRH =0x06; //重新装载104us，确保在后续的bit中间位置读取数据
//	TIM2ARRL =0x80; 
//    T2CEN=1;        //使能定时器2
//	while(i--)
//	{
//		Output =Output>>1;
//		if(RXIO) 
//        {
//        	Output   |=0x80;      			//先接收收低位
//        }
//		WaitTF0();                 			//位间延时104us
//	}
//	T2UIE=0;                          		//停止Timer0
//	return Output;                          //返回接收到的byte
//}
// 
// 
//u8 Rx_buff[Rx_size]={0};
//volatile bit B_Rx_decode;
//void UART_IO_RxProcess(vu8 *buff,vu8 size)//接收size长度的数据放在buff中
//{
//    u8 rdata;
//    static u8 cnt,checksum;
//	if(RXFLAG)							//RXFLAG在下降沿外部中断函数中置1
//    {
//        rdata=RByte();                  //有下降沿中断说明有起始位，开始接收数据
//        if(!Rx_Tim)//倒计时结束，说明超过50ms没有接收到数据，清零接收计数值，开始接收下一帧.
//        {
//          cnt=0;
//          checksum=0;
//        }
//        Set_Clear_RxTim();//接收到数据，复位倒计时
//        buff[cnt]=rdata;
//        if(cnt<(Rx_size-2))
//            checksum+=buff[cnt];//计算数据校验和，与接收到的校验和对比。
//         if(cnt==(Rx_size-1))//计数值与数据长度相等，说明接收完一帧数据。
//        {
//            if((buff[0]==d_head)&&(buff[Rx_size-1]==d_tail)&&(checksum==buff[Rx_size-2]))
//				B_Rx_decode=1;//帧头，帧尾，校验和,全部正确，接收数据有效。
//        }
//		cnt++;
//        EPIE0   = 0B00000010;//接收完一个数据，重新使能外部下降沿中断，准备接收下一个数据
//		RXFLAG = 0;          //标志位清零
//    } 
// }
// 
//u8 Tx_buff[Tx_size]={0};
//void UART_IO_TxProcess(vu8 *buff,vu8 size)//发送数据
//{
//	u8 i;
//    for(i=0;i<size;i++)
//    {
//        WByte(buff[i]);
//    }
//}



void jieshou_try(int8 x)
{uint8 i=1;
	while(1)
{
uart_query_byte(UART_4,&jieshoushuju.map_data);

if(jieshoushuju.map_data!=jieshoushuju.add[i-1])
{ 
	jieshoushuju.add[i]=jieshoushuju.map_data;
	i++;
	}
if(i==x+1)
{	i=1;
	jieshoushuju.map_data=0;
	break;}
}}