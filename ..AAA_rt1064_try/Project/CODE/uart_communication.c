//uart����ͨ��
//
//author��czy
//
//����޸�ʱ�䣺2023.4.2


#include "uart_communication.h"
//#define LED1                    (B9 )  //�������ã�����tryʱע��

uint8 							lineate_uart_rx_buffer[LINEATE_UART_BUFFER_SIZE];  //������ջ���������ǰֱ��ʹ��Ĭ�ϻ���������
uint8 							lineate_uart_rx_buffer_len;  //���ջ���������


//uint8              lineate_uart_tx_buffer;  //���ͻ��������ٲ��ַ��Ϳ����Ȳ��û�����
//uint8 							lineate_uart_rx_buffer_len;  //���ͻ��������ȣ��ٲ��ַ��Ϳ����Ȳ��û�����


static fifo_struct lineate_uart_fifo;  //����һ��FIFO����ָ�룬��Ϊ�ҵ�user_fifo���ڽ�����FIFO ��ʼ�� ���ض�Ӧ��������ʹ�ã�fifo_init��

static          uint8       lineate_uart_data;  //�ص������ж��������ݴ�ţ��ٷ��뻺����ʱ��

 jieshou_try_need jieshoushuju;

//uint8 data_buffer[32];  //�������ã�����tryʱע��
//uint8 data_len;  //�������ã�����tryʱע��


//bool data_head = 0;
//bool data_last = 0;

 


//-------------------------------------------------------------------------------------------------------------------
// �������     �������ߴ���д�ĳ�ʼ����������ģʽ��uart�������������ʹ��
// ����˵��     uint8
// ���ز���     return_state
// ʹ��ʾ��     lineate_uart_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

uint8 lineate_uart_init(void)
{
	uint8 return_state = 0;
	
	set_wireless_type(LINEATE, lineate_uart_callback);   //������жϻص�����������wireless_uart_callback������ʱע�⣡����, ͬʱע������ģ������ʱѡ������ģ�飬��֪���᲻������⣬�������⣬�ص㸴������
	//(����ܴ���ܵ��Լ�дһ��lineate��ģʽ�ص���������Ϊ��������NO_WIRELESS�ƺ�һ�������type_default_callback�����ص���void)
	
	
	//FIFO���ã����տɼӣ����ͻ�����
	
	fifo_init(&lineate_uart_fifo,FIFO_DATA_8BIT,lineate_uart_rx_buffer, LINEATE_UART_BUFFER_SIZE);    //��ʼ��FIFO��������FIFO ����ָ��Ϊlineate_uart_fifo������λ��Ϊ8���أ����صĻ�����Ϊlineate_uart_rx_buffer���䳤��LINEATE_UART_BUFFER_SIZE����.h�ļ��и���
  gpio_init(LINEATE_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);              //��ʼ��gpio���������ΪLINEATE_UART_RTS_PIN��Ĭ�ϸߵ�ƽ �������ģʽ
#if(0 == LINEATE_UART_AUTO_BAUD_RATE)                                          // �ر��Զ�������
    // ������ʹ�õĲ�����Ϊ115200 Ϊ����ת����ģ���Ĭ�ϲ����� ������������������������ģ�鲢�޸Ĵ��ڵĲ�����
		uart_init(LINEATE_UART_INDEX, LINEATE_UART_BUAD_RATE, LINEATE_UART_RX_PIN, LINEATE_UART_TX_PIN);  //���ڿ����żǵø�һ�£���ʼ��uart�����뷢������
	
//    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_interrupt(LINEATE_UART_INDEX, 1);					// �򿪴���1��������ж�
#elif(1 == LINEATE_UART_AUTO_BAUD_RATE)                                        // �����Զ�������
    uint8 rts_init_status;
    uint16 time_count = 0;

    wireless_auto_baud_flag = 0;
    wireless_auto_baud_data[0] = 0;
    wireless_auto_baud_data[1] = 1;
    wireless_auto_baud_data[2] = 3;

    rts_init_status = gpio_get_level(WIRELESS_UART_RTS_PIN);
    gpio_init(WIRELESS_UART_RTS_PIN, GPO, rts_init_status, GPO_PUSH_PULL);      // ��ʼ����������

    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);

    system_delay_ms(5);                                                         // ģ���ϵ�֮����Ҫ��ʱ�ȴ�
    gpio_set_level(WIRELESS_UART_RTS_PIN, !rts_init_status);                    // RTS�������ߣ������Զ�������ģʽ
    system_delay_ms(100);                                                       // RTS����֮�������ʱ20ms
    gpio_toggle(WIRELESS_UART_RTS_PIN);                                         // RTS����ȡ��

    wireless_auto_baud_flag = 1;

    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[0]);           // �����ض����� ����ģ���Զ��жϲ�����
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[1]);           // �����ض����� ����ģ���Զ��жϲ�����
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[2]);           // �����ض����� ����ģ���Զ��жϲ�����
    system_delay_ms(20);

    time_count = 0;
    do
    {
        if(3 != wireless_auto_baud_flag)                                        // �����Զ��������Ƿ����
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // ���������뵽������� ˵���Զ�������ʧ����
            break;
        }

        time_count = 0;
        if( 0xa5 != wireless_auto_baud_data[0] &&                               // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[1] &&                               // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[2] )                                // �����Զ��������Ƿ���ȷ
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // ���������뵽������� ˵���Զ�������ʧ����
            break;
        }
        wireless_auto_baud_flag = 0;

        gpio_init(WIRELESS_UART_RTS_PIN, GPI, 0, GPI_PULL_UP);                  // ��ʼ����������
        system_delay_ms(10);                                                    // ��ʱ�ȴ� ģ��׼������
    }while(0);
#endif
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ���ڽ��ջ�����
// ����˵��     uint32 
// ���ز���     data_len
// ʹ��ʾ��     lineate_uart_try();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

uint32 lineate_uart_buff_read(uint8 *buff, uint32 len)
{
	zf_assert(buff != NULL);
    uint32 data_len = len;
	
    fifo_read_buffer(&lineate_uart_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);  //ģʽΪFIFO_READ_AND_CLEAN����ȡ���������������ȡ�����ݣ�buffΪĿ�껺����
	
    return data_len;
	
}



//-------------------------------------------------------------------------------------------------------------------
// �������     ���ʹ��ڷ����ֽ�
// ����˵��     uint32 
// ���ز���     data_len
// ʹ��ʾ��     lineate_uart_try();
// ��ע��Ϣ     
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
// �������     ���Ժ��������������չ����Ƿ����ʹ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     lineate_uart_try();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

void lineate_uart_try(void)
{
    uint8 count = 0;

//    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
	
    if(lineate_uart_init())                                                    // �ж��Ƿ�ͨ����ʼ��
    {
        while(1)                                                                // ��ʼ��ʧ�ܾ����������ѭ��
        {
//            gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ����
					tft180_show_string(0,60,"lineate_uart_init error");
          system_delay_ms(100);                                               // ��ʾ�쳣
        }
    }
		while(1){
			tft180_show_string(0,60,"lineate_uart_init");
//			lineate_uart_send_byte('\r');
			uart_write_byte(UART_4,1);
			
		}
//    lineate_uart_send_byte('\n');
//    lineate_uart_send_string("SEEKFREE wireless uart demo.\r\n");              // ��ʼ������ ���������Ϣ
    
//    interrupt_global_enable(0);
    
    // �˴���д�û����� ���������ʼ�������

//    while(1)
//    {
//        // �˴���д��Ҫѭ��ִ�еĴ���
//        data_len = wireless_uart_read_buff(data_buffer, 32);                    // �鿴�Ƿ�����Ϣ Ĭ�ϻ������� WIRELESS_UART_BUFFER_SIZE �ܹ� 64 �ֽ�
//        if(data_len != 0)                                                       // �յ�����Ϣ ��ȡ�����᷵��ʵ�ʶ�ȡ�������ݸ���
//        {
//            wireless_uart_send_buff(data_buffer, data_len);                     // ���յ�����Ϣ���ͻ�ȥ
//            memset(data_buffer, 0, 32);
//            func_uint_to_str((char *)data_buffer, data_len);
//            wireless_uart_send_string("\r\ndata len:");                                 // ��ʾʵ���յ���������Ϣ
//            wireless_uart_send_buff(data_buffer, strlen((const char *)data_buffer));    // ��ʾ�յ������ݸ���
//            wireless_uart_send_string(".\r\n");
//        }
////        if(count ++ > 10)
////        {
////            count = 0;
////            gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ����
////        }
//        system_delay_ms(50);
//    }


	}		
//		
		
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ģʽ���ڻص�����
// ����˵��     uint32 
// ���ز���     data_len
// ʹ��ʾ��     lineate_uart_try();
// ��ע��Ϣ     �����ص�������Ŀǰ���ڲ���ʹ�ã����ڸ���Ҫ���͵����ݸ��Ĵ��빦��
//-------------------------------------------------------------------------------------------------------------------
	
void lineate_uart_callback (void)
{
	uart_query_byte(LINEATE_UART_INDEX, &lineate_uart_data);

	fifo_write_buffer(&lineate_uart_fifo, &lineate_uart_data, 1);
	
#if WIRELESS_UART_AUTO_BAUD_RATE                                                // �����Զ�������
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8 *)wireless_auto_baud_data, (uint32 *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif


}

//-------------------------------------------------------------------------------------------------------------------

	
//˵�������´���Ϊ�ο�����֮һ����Դ��CSDN�������ο�	
	
	
//-------------------------------------------------------------------------------------------------------------------
	
	
////UART��������9600,1 bit��104us��
////1����ʼλ��1��ֹͣλ��8������λ
////ÿ�����ݰ�֡ͷ��0xAA��֡β0x0D��У��ֵ�ǳ���֡β�������ݵ��ۼӺ͡�
////ͨѶ��ʽ����һ��һ����ʽ��ÿ500ms����һ�����ݲ�����Ӧ�����ݣ�
////���������ܹ�22byte��22*(1+1+8)*104us=22.88ms,������ÿ֡���ݵ�ʱ�䳤��.
// 
//#define d_head  0xAA
//#define d_tail  0x0D
// 
//volatile bit RXFLAG = 0;//�ⲿ�жϱ�־λ,���½����ж�����1;
//u8	Rx_Tim=0;    //���ռ�ʱ����ÿ�ν��յ�����ʱ���õ���ʱ������ʱΪ0����Ϊ��һ֡����,        
//                 //��ʼ������һ֡���ݣ���λ���ռ�������
//#define Set_Clear_RxTim()        Rx_Tim=50    //50ms����ʱ���ҵĲ�Ʒ��������ʱ�䳤��22.88ms
//                                              //������23ms�Ϳ����ˣ�����Ҫ����ʱ�������
//                
//void Uart_IO_GPIO_Init(void)//ģ�⴮�ڵ�IO�ڳ�ʼ��
//{
//    PORTA|= 0B00000001;		//Tx���1
//    TRISA &= 0B11111110;    //PA0-Output_Tx  PA1-Input_Rx
//    WPUA  |= 0B00000010;    //PA1��������
//}
// 
//void WaitTF0(void)//��ʱ�ȴ�����UARTʱ��
//{
//     while(T2UIE);//��ʱ����ʼ��ʱ���ڶ�ʱ���ж�������T2UIE
//     T2UIE=1;
//}
// 
//void WByte(u8 input)//����1 Byte
//{
//	u8 i=8;
//	TXIO=0;     //Tx��0��������ʼλ
//    T2UIF = 1;  //���㶨ʱ��2�жϱ�־λ
//	T2CEN =0;   //ʧ�ܶ�ʱ��2
//	TIM2CNTRH=0;//���㶨ʱ��2����ֵ��λ
//	TIM2CNTRL=0;//���㶨ʱ��2����ֵ��λ
//	T2CEN=1;    //������ʱ��2
//	T2UIE =1;   //ʹ�ܶ�ʱ��2
//	WaitTF0();  //�ȴ���ʱ���жϣ��ȴ�ʱ��ȡ���ڶ�ʱ��2��װ��ֵ������Ķ�ʱʱ����104us
//	while(i--)
//	{
//		if(input&0x01) //���bit���ͣ��ȴ���λ
//			TXIO=1;
//		else
//			TXIO = 0;   
//		WaitTF0(); //�ȴ���ʱ104us
//		input=input>>1; 
//	}
//	TXIO=1;      //���ͽ���λ
//	WaitTF0();
//	T2UIE=0;     //����1 byte�����     
//}
// 
// 
//u8 RByte()//����1 Byte
//{
//	u8 Output=0;
//	u8 i=8;
//    T2UIF = 1;    //���㶨ʱ��2�жϱ�־λ
//    T2CEN=0;      //ʧ�ܶ�ʱ��2
//	TIM2CNTRH=0;  //���㶨ʱ��2����ֵ��λ
//	TIM2CNTRL=0;  //���㶨ʱ��2����ֵ��λ
//    TIM2ARRH =0x09; //�趨ʱ��2װ��ֵ
//	TIM2ARRL =0x00;	//��ʼλ1.5bit(156us)���ٶ�ȡ��ƽ��ȷ���ڵ�һbit�м�λ�ö�ȡ����
//    T2CEN=1;        //������ʱ��2                
//    T2UIE =1;       //ʹ�ܶ�ʱ��2
//	WaitTF0();      //�ȴ�104us+52us=156us���պ��ڵ�һ��bit���м�λ�ö�ȡ����
//    T2CEN=0;        //ʧ�ܶ�ʱ��2
//  	TIM2ARRH =0x06; //����װ��104us��ȷ���ں�����bit�м�λ�ö�ȡ����
//	TIM2ARRL =0x80; 
//    T2CEN=1;        //ʹ�ܶ�ʱ��2
//	while(i--)
//	{
//		Output =Output>>1;
//		if(RXIO) 
//        {
//        	Output   |=0x80;      			//�Ƚ����յ�λ
//        }
//		WaitTF0();                 			//λ����ʱ104us
//	}
//	T2UIE=0;                          		//ֹͣTimer0
//	return Output;                          //���ؽ��յ���byte
//}
// 
// 
//u8 Rx_buff[Rx_size]={0};
//volatile bit B_Rx_decode;
//void UART_IO_RxProcess(vu8 *buff,vu8 size)//����size���ȵ����ݷ���buff��
//{
//    u8 rdata;
//    static u8 cnt,checksum;
//	if(RXFLAG)							//RXFLAG���½����ⲿ�жϺ�������1
//    {
//        rdata=RByte();                  //���½����ж�˵������ʼλ����ʼ��������
//        if(!Rx_Tim)//����ʱ������˵������50msû�н��յ����ݣ�������ռ���ֵ����ʼ������һ֡.
//        {
//          cnt=0;
//          checksum=0;
//        }
//        Set_Clear_RxTim();//���յ����ݣ���λ����ʱ
//        buff[cnt]=rdata;
//        if(cnt<(Rx_size-2))
//            checksum+=buff[cnt];//��������У��ͣ�����յ���У��ͶԱȡ�
//         if(cnt==(Rx_size-1))//����ֵ�����ݳ�����ȣ�˵��������һ֡���ݡ�
//        {
//            if((buff[0]==d_head)&&(buff[Rx_size-1]==d_tail)&&(checksum==buff[Rx_size-2]))
//				B_Rx_decode=1;//֡ͷ��֡β��У���,ȫ����ȷ������������Ч��
//        }
//		cnt++;
//        EPIE0   = 0B00000010;//������һ�����ݣ�����ʹ���ⲿ�½����жϣ�׼��������һ������
//		RXFLAG = 0;          //��־λ����
//    } 
// }
// 
//u8 Tx_buff[Tx_size]={0};
//void UART_IO_TxProcess(vu8 *buff,vu8 size)//��������
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