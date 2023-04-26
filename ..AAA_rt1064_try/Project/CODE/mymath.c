#include "mymath.h"

#include<stdio.h>
#include<stdlib.h>

void convolution2(int* input1, int* input2, int* output, int mm, int nn)
{
	//�洢��ַ
	int* xx = (int*)malloc(sizeof(int) * (mm + nn - 1));
	for (int i = 0; i < mm + nn - 1; i++) {
		xx[i] = 0;
	}

	//��ʼ���
	//����ʱ��Ч������¼���г˻���ʱ��λ��һ�������
	for (int i = 0; i < mm; i++) {
		for (int j = 0; j < nn; j++) {
			xx[i + j] += input1[i] * input2[j];
		}
	}

	for (int i = 0; i < mm + nn - 1; i++) {
		output[i] = xx[i];
	}
//	delete[] xx;
}

//int main() {
//	int mm, nn;
//	printf("�������һ�������Ŀ\n");
////	scanf_s("%d", &mm);

//	//malloc����ռ�
//	int* input1 = (int*)malloc(sizeof(int) * mm);
//	printf("�������һ�������ֵ\n");
//	for (int i = 0; i < mm; i++) {
////		scanf_s("%d", &input1[i]);
//	}
//	printf("������ڶ��������Ŀ\n");
////	scanf_s("%d", &nn);
//	int* input2 = (int*)malloc(sizeof(int) * nn);
//	printf("������ڶ��������ֵ\n");
//	for (int j = 0; j < nn; j++) {
////		scanf_s("%d", &input2[j]);
//	}

//	//������ռ�
//	int* output = (int*)malloc(sizeof(int) * (mm + nn - 1));
//	convolution2(input1, input2, output, mm, nn);

//	printf("������\n");
//	for (int i = 0; i < mm + nn - 1; i++)
//	{
//		printf("%d ", output[i]);
//	}
//	printf("\n");

//	return 0;
//}




