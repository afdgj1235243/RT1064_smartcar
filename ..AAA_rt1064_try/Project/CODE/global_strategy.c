/*ȫ�ֹ滮
2023/7/1
by С��Ź���*/

#include "global_strategy.h"

int remain_Num=6;

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
				if(abs(Car.MileageX) < 20*abs(car_test_x)){
				//if(abs(Car.MileageY) < 20*abs(car_test_y)){
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
					if(car_test_x == 0){			
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
					if(car_test_x == 0){			
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

void remain()     //�̶�����ʶ����ɣ���ʼ��Ѱɢ���������ͼƬ*****������������
{
	 if(ax_add_test[11] < 18)
	{
		remain_planning1();
	}else{
		remain_planning2();
	}		
}