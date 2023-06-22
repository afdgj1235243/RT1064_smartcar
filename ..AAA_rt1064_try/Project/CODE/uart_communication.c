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

uint8               openart_rx_buffer;
lpuart_transfer_t   openart_receivexfer;
lpuart_handle_t     openart_g_lpuartHandle;

uint8               openart1_rx_buffer;
lpuart_transfer_t   openart1_receivexfer;
lpuart_handle_t     openart1_g_lpuartHandle;

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ģʽ��uart�������������ʹ��
// ����˵��     uint8
// ���ز���     return_state
// ʹ��ʾ��     lineate_uart_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------

uint8 lineate_uart_init(void)
{
	uint8 return_state = 0;
	
	set_lineate_type(LINEATE, lineate_uart_callback);   

	//FIFO���ã����տɼӣ����ͻ�����

	fifo_init(&lineate_uart_fifo,FIFO_DATA_8BIT,lineate_uart_rx_buffer, LINEATE_UART_BUFFER_SIZE);    //��ʼ��FIFO��������FIFO ����ָ��Ϊlineate_uart_fifo������λ��Ϊ8���أ����صĻ�����Ϊlineate_uart_rx_buffer���䳤��LINEATE_UART_BUFFER_SIZE����.h�ļ��и���
	
//  gpio_init(LINEATE_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);              //��ʼ��gpio���������ΪLINEATE_UART_RTS_PIN��Ĭ�ϸߵ�ƽ �������ģʽ
#if(0 == LINEATE_UART_AUTO_BAUD_RATE)                                          // �ر��Զ�������
    // ������ʹ�õĲ�����Ϊ115200 Ϊ����ת����ģ���Ĭ�ϲ����� ������������������������ģ�鲢�޸Ĵ��ڵĲ�����
		uart_init(LINEATE_UART_INDEX, LINEATE_UART_BUAD_RATE, LINEATE_UART_RX_PIN, LINEATE_UART_TX_PIN);  //���ڿ����żǵø�һ�£���ʼ��uart�����뷢������
		
		tft180_show_string(0,60,"error");
	
		NVIC_SetPriority((IRQn_Type)((IRQn_Type)(LINEATE_UART_INDEX) + LPUART1_IRQn),0); 
	
    uart_rx_interrupt(LINEATE_UART_INDEX, 1);					// �򿪴���1��������ж�
	
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




//��ʱͨ���ô��룬lensΪ�Լ�ȷ�����������
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

