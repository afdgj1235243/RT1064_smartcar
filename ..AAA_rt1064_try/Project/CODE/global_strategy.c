/*ȫ�ֹ滮
2023/7/1
by С��Ź���*/

#include "global_strategy.h"

int remain_Num=6;
int stop_num_text =0;
int car_stop_x,car_stop_y;
int unloadx[6], unloady[6];
int car_stop_x = 30,car_stop_y = 5;
int car_last_x ,car_last_y ;
int car_last_x ,car_last_y ;
extern int ax_add_test[12],ay_add_test[12];        
extern location_goal Car;

void remain_planning1()//���һ���������г����
{
			int planning_Num=0;
			int planningx[11] = {34,34,1,1,34,34,1,1,34,34,1};
			int planningy[11] = {1,5,5,9,9,13,13,17,17,21,21};		
			int car_last_x=1,car_last_y=1;
			int8 car_test_x,car_test_y;
			car_test_x = 1 - ax_add_test[11];
			car_test_y = 1 - ay_add_test[11];
//���ѭ���ǽ�����������1��1��			
				while(1){	
					if(car_test_x != 0 && abs(car_test_x) != 1){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //������X=0�����������ʹ�õģ���Ҫ����д����shitɽ����
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}
				}	
//��ʼ����ȫͼ��Ѱ
		while(remain_Num)
		{
				if(planning_Num < 11 && remain_Num > 0){					     				
					car_test_x = ax_add_test[planning_Num] - car_last_x;
					car_test_y = ay_add_test[planning_Num] - car_last_y;								
					car_last_x = ax_add_test[planning_Num];
					car_last_y = ay_add_test[planning_Num];				
					planning_Num ++;				
				}
				while(1){	
					if(car_test_x != 0 && abs(car_test_x) != 1){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //������X=0�����������ʹ�õģ���Ҫ����д����shitɽ����
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}		
		/*���ʶ���˰���
			�Ӹ�if�жϣ�Ȼ��--
			 remain_Num--;//ʶ��һ��Ŀ���--
		*/
			}
		}
}


void remain_planning2()
{
				int planning_Num=0;
			int planningx[11] = {1,1,34,34,1,1,34,34,1,1,34};
			int planningy[11] = {1,5,5,9,9,13,13,17,17,21,21};		
			int car_last_x=1,car_last_y=1;
			int8 car_test_x,car_test_y;
			car_test_x = 34 - ax_add_test[11];
			car_test_y = 1 - ay_add_test[11];
//���ѭ���ǽ�����������34��1��			
				while(1){	
					if(car_test_x != 0 && abs(car_test_x) != 1){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //������X=0�����������ʹ�õģ���Ҫ����д����shitɽ����
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}
				}	
//��ʼ����ȫͼ��Ѱ
		while(remain_Num)
		{
				if(planning_Num < 11 && remain_Num > 0){					     				
					car_test_x = ax_add_test[planning_Num] - car_last_x;
					car_test_y = ay_add_test[planning_Num] - car_last_y;								
					car_last_x = ax_add_test[planning_Num];
					car_last_y = ay_add_test[planning_Num];				
					planning_Num ++;				
				}
				while(1){	
					if(car_test_x != 0 && abs(car_test_x) != 1){			
						if(abs(Car.MileageX) < 20*abs(car_test_x)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				  }else{              //������X=0�����������ʹ�õģ���Ҫ����д����shitɽ����
							if(abs(Car.MileageY) < 20*abs(car_test_y)){
								car_omni(car_test_x,car_test_y,Car.Speed_Z);			
							}
							else{
								car_stop();
								icm20602_init();
								encoder_init();
								system_delay_ms(1000);
								break;
					  }
				}		
		/*���ʶ���˰���
			�Ӹ�if�жϣ�Ȼ��--
			 remain_Num--;//ʶ��һ��Ŀ���--
		*/
			}
		}
}

void unload_left()//�����ڵ�ͼ��ߵ�ж��
{
				while(1){	
			if(abs(Car.MileageX) < 20){
				car_omni(-1,0,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(1000);
					break;
			}
		}	
				//����
				while(1){	
			if(abs(Car.MileageX) < 0){
				car_omni(1,0,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(2000);
					break;
			}
		}						
}

void unload_right()//�����ڵ�ͼ�ұߵ�ж��
{
				while(1){	
			if(abs(Car.MileageX) < 20){
				car_omni(1,0,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(1000);
					break;
			}
		}		
				//����
				while(1){	
			if(abs(Car.MileageX) < 20){
				car_omni(-1,0,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(2000);
					break;
			}
		}					
}

void unload_above()//�����ڵ�ͼ�ϱߵ�ж��
{
				while(1){	
			if(abs(Car.MileageY) < 20){
				car_omni(0,1,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(1000);
					break;
			}
		}	
				//����
				while(1){	
			if(abs(Car.MileageY) < 20){
				car_omni(0,-1,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(2000);
					break;
			}
		}						
}

void unload_below()//�����ڵ�ͼ�±ߵ�ж��
{
				while(1){	
			if(abs(Car.MileageY) < 20){
				car_omni(0,-1,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(1000);
					break;
			}
		}			
				//����
					while(1){	
			if(abs(Car.MileageY) < 20){
				car_omni(0,1,Car.Speed_Z);			
			}
				else{
					car_stop();
					icm20602_init();
					encoder_init();
					system_delay_ms(2000);
					break;
			}
		}	
}

void unload_move(int point)//ж���ƶ�·���滮
{	
 int8 car_test_x ,car_test_y;
    if (car_stop_x < 18) //ж��·��---���ϣ��ң��� //car_stop_x == 1
    {
        unloadx[0] = 1;
        unloadx[1] = 15;
        unloadx[2] = 15;
        unloadx[3] = 35;
        unloadx[4] = 31;
        unloadx[5] = 1;
        
        unloady[0] = car_stop_y;
        unloady[1] = car_stop_y;
        unloady[2] = 25;
        unloady[3] = 5;
        unloady[4] = 1;
        unloady[5] = 1;
    }
    else
    {  
        unloadx[0] = 35;
        unloadx[1] = 15;
        unloadx[2] = 15;
        unloadx[3] = 15;
        unloadx[4] = 1;
        unloadx[5] = 1;

        unloady[0] = car_stop_y;
        unloady[1] = car_stop_y;
        unloady[2] = 25;
        unloady[3] = 1;
        unloady[4] = 15;
        unloady[5] = 1;
    }
	
			if(stop_num_text < point ){					     				
				car_test_x = unloadx[stop_num_text] - car_last_x;
				car_test_y = unloady[stop_num_text] - car_last_y;								
				car_last_x = unloadx[stop_num_text];
				car_last_y = unloady[stop_num_text];				
				stop_num_text ++;				
			}
			while(1){	
				if(car_test_x != 0 && abs(car_test_x) != 1){			
					if(abs(Car.MileageX) < 20*abs(car_test_x)){
						car_omni(car_test_x,car_test_y,Car.Speed_Z);			
					}
						else{
							car_stop();
							icm20602_init();
							encoder_init();
							system_delay_ms(1000);
							break;
					}
				}else{              //������X=0�����������ʹ�õģ���Ҫ����д����shitɽ����
						if(abs(Car.MileageY) < 20*abs(car_test_y)){
							car_omni(car_test_x,car_test_y,Car.Speed_Z);			
						}
						else{
							car_stop();
							icm20602_init();
							encoder_init();
							system_delay_ms(1000);
							break;
					}
			  }			
		}

}


void remain()     //�̶�����ʶ����ɣ���ʼ��Ѱɢ���������ͼƬ*****������������
{
	 if(ax_add_test[11] < 18)
	{
		remain_planning1();
	}else{
		remain_planning2();
	}		
}


void unload()
{
	int stop_num = 6;
  car_last_x = car_stop_x,car_last_y = car_stop_y;
			if( car_stop_x < 18 )
//		if( ax_add_test[11] < 18 ) //car_stop_x == 1
		{
				while (stop_num)
			{
				if (stop_num == 5)
				{
						// �����ڵ�ͼ���ʱ��ж��
						unload_left();
				}
				else if (stop_num == 3)
				{
						// �����ڵ�ͼ�ϱ�ʱ��ж��
						unload_above();
				}
				else if (stop_num == 2)
				{
						// �����ڵ�ͼ�ұ�ʱ��ж��
						unload_right();
				}
				else if (stop_num == 1)
				{
						// �����ڵ�ͼ�±�ʱ��ж��
						unload_below();
				}
			unload_move(6);
			stop_num --;
			}
		}else{
				while (stop_num)
			{
				if (stop_num == 5)
				{
						// �����ڵ�ͼ�ұ�ʱ��ж��
					unload_right();
				}
				else if (stop_num == 3)
				{
						// �����ڵ�ͼ�ϱ�ʱ��ж��
					unload_above();
				}
				else if (stop_num == 2)
				{
						// �����ڵ�ͼ�±�ʱ��ж��
					unload_below();
				}
				else if (stop_num == 1)
				{
						// �����ڵ�ͼ���ʱ��ж��
					unload_left();
				}
			unload_move(6);
			stop_num --;
			}
		} 
}