/*�Ŵ��㷨·���滮*/

#include "path_planning.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUM_CITIES 13    // �����������������1��1
#define NUM_ANTS 50    // �������� 
#define ALPHA 1.0			// ������Ϣ����Ҫ�̶����ӣ�
#define BETA 5.0         // �£�����ʽ���ӣ�
#define RHO 0.5          // �ѣ���Ϣ�ػӷ�ϵ����
#define Q 100            // Q����Ϣ����ǿǿ�ȣ�
#define MAX_ITERATIONS 100

extern jieshou_try_need jieshoushuju;

typedef struct {
    int x;    // ���е�x����
    int y;    // ���е�y����
} City;

	extern int8 x_add_test[12];
	extern int8 y_add_test[12];

int ax_add_test[12] = {5,10,5,12,17,33,6,22,13,29,5,33};   // ���е�x�������飬���� NUM_CITIES ��Ԫ��
int ay_add_test[12] = {5,5,10,10,12,13,15,16,18,21,22,23};     // ���е�y�������飬���� NUM_CITIES ��Ԫ��

double distances[NUM_CITIES][NUM_CITIES];      // �洢���м����Ķ�ά����
double pheromones[NUM_CITIES][NUM_CITIES];     // �洢��Ϣ��Ũ�ȵĶ�ά����

void initializeDistances(City cities[]) {
    // ���ݳ���֮���ŷ����þ������������
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            distances[i][j] = sqrt(dx * dx + dy * dy);
        }
    }
}

void initializePheromones() {
    // ��ʼ����Ϣ�ؾ��󣬽���ȫ������Ϊһ����ʼŨ��ֵ
    double initialPheromone = 1.0 / ((double)NUM_CITIES * (double)distances[0][NUM_CITIES - 1]);

    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] = initialPheromone;
        }
    }
}

// �жϳ����Ƿ�Ϸ�
int isValidCity(int city, int visited[]) {
    return (city >= 0 && city < NUM_CITIES && visited[city] == 0);
}

// ѡ����һ��Ҫ���ʵĳ���
int selectNextCity(int ant, int currCity, int visited[]) {
    double total = 0.0;
    double probabilities[NUM_CITIES] = { 0.0 };

    // ������Ϣ�غ�����ʽ���Ӽ���ÿ��δ���ʳ��еĸ���
    for (int i = 0; i < NUM_CITIES; i++) {
        if (isValidCity(i, visited)) {
            double pheromone = pow(pheromones[currCity][i], ALPHA);
            double distance = pow(1.0 / distances[currCity][i], BETA);
            total += pheromone * distance;
            probabilities[i] = total;
        }
        else {
            probabilities[i] = 0.0;
        }
    }

    // ����һ��������������ݸ���ѡ����һ������
    double random = (double)rand() / RAND_MAX * total;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (probabilities[i] > random && isValidCity(i, visited)) {
            return i;
        }
    }

    return -1;
}

// ������Ϣ�ؾ���
void updatePheromones(int antSolutions[][NUM_CITIES], double deltaPheromones[][NUM_CITIES]) {
    int globalBestAntIndex = -1;
    double globalBestLength = -1.0;

    // ����ÿֻ���ϵĽ⣬��������Ϣ��
    for (int ant = 0; ant < NUM_ANTS; ant++) {
        double solutionLength = 0.0;

        // ����ÿ�����·������
        for (int i = 0; i < NUM_CITIES - 1; i++) {
            int city1 = antSolutions[ant][i];
            int city2 = antSolutions[ant][i + 1];
            solutionLength += distances[city1][city2];
        }
        solutionLength += distances[antSolutions[ant][NUM_CITIES - 1]][antSolutions[ant][0]];

        // ����ȫ������·���ͳ���
        if (globalBestLength == -1 || solutionLength < globalBestLength) {
            globalBestLength = solutionLength;
            globalBestAntIndex = ant;
        }

        // ������Ϣ��������
        for (int i = 0; i < NUM_CITIES - 1; i++) {
            int city1 = antSolutions[ant][i];
            int city2 = antSolutions[ant][i + 1];
            deltaPheromones[city1][city2] += Q / solutionLength;
            deltaPheromones[city2][city1] += Q / solutionLength;
        }
    }

    // ����ȫ������·������Ϣ��������
    for (int i = 0; i < NUM_CITIES - 1; i++) {
        int city1 = antSolutions[globalBestAntIndex][i];
        int city2 = antSolutions[globalBestAntIndex][i + 1];
        deltaPheromones[city1][city2] += Q / globalBestLength;
        deltaPheromones[city2][city1] += Q / globalBestLength;
    }

    // ����ȫ����Ϣ�ؾ���
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] *= (1 - RHO);
            pheromones[i][j] += deltaPheromones[i][j];
        }
    }
}

// ��Ⱥ�㷨�Ż�
int antColonyOptimization(City cities[], int optimalPath[], double antLengths[]) {
    initializeDistances(cities);
    initializePheromones();

    int antSolutions[NUM_ANTS][NUM_CITIES];

    // ���� MAX_ITERATIONS ��
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double deltaPheromones[NUM_CITIES][NUM_CITIES] = { 0.0 };

        // ÿֻ���Ϲ���һ���⣬��������·������
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            int visited[NUM_CITIES] = { 0 };
            int currCity = 0;
            visited[currCity] = 1;
            antSolutions[ant][0] = currCity;

            // ���ݸ���ѡ����һ��Ҫ���ʵĳ��У�ֱ�����������г���
            for (int i = 1; i < NUM_CITIES; i++) {
                int nextCity = selectNextCity(ant, currCity, visited);

                if (nextCity == -1) {
                    break;
                }

                antSolutions[ant][i] = nextCity;
                visited[nextCity] = 1;
                currCity = nextCity;
            }

            // ���㵱ǰ���·������
            double solutionLength = 0.0;
            for (int i = 0; i < NUM_CITIES - 1; i++) {
                int city1 = antSolutions[ant][i];
                int city2 = antSolutions[ant][i + 1];
                solutionLength += distances[city1][city2];
            }
            solutionLength += distances[antSolutions[ant][NUM_CITIES - 1]][antSolutions[ant][0]];
            antLengths[ant] = solutionLength;
        }

        // ������Ϣ�ؾ���
        updatePheromones(antSolutions, deltaPheromones);
    }

    int globalBestAntIndex = -1;
    double globalBestLength = -1.0;

    // �ҵ�ȫ������·��������
    for (int ant = 0; ant < NUM_ANTS; ant++) {
   // ����ȫ������·���ͳ���
    if (globalBestLength == -1 || antLengths[ant] < globalBestLength) {
        globalBestLength = antLengths[ant];
        globalBestAntIndex = ant;
			}
		}
		// ��ȫ������·�����浽 optimalPath ������
		for (int i = 0; i < NUM_CITIES; i++) {
    optimalPath[i] = antSolutions[globalBestAntIndex][i];
	}
	return globalBestLength;
}

void path() {
	
    City cities[NUM_CITIES];//��ʼ����������
    int optimalPath[NUM_CITIES];//��������·�������� 
    double antLengths[NUM_ANTS];//����ÿֻ���ϵ�·������

    cities[0].x = 1;
    cities[0].y = 1;   //��ʼ��

		
	
    for (int i = 1; i < NUM_CITIES; i++) {
        cities[i].x = ax_add_test[i-1];
        cities[i].y = ay_add_test[i-1];
    }

    int bestAntIndex = antColonyOptimization(cities, optimalPath, antLengths);

//    printf("����·����");
//    for (int i = 0; i < 4; i++) {
//			tft180_show_int(0,15*i,optimalPath[i],3);
//			tft180_show_int(30,15*i,optimalPath[i+4],3);
//			tft180_show_int(60,15*i,optimalPath[i+8],3);
//			tft180_show_int(90,15*i,optimalPath[i+12],3);
//        printf("%d ", optimalPath[i]);
//    }
//    printf("\n");

		//����
		
//	printf("�������к�� ax_add_test:");
//    for (int i = 1; i < NUM_CITIES ; i++) {
//       jieshoushuju.X[i-1] = cities[optimalPath[i]].x;
//       jieshoushuju.Y[i-1] = cities[optimalPath[i]].y;
//    }
		
		    for (int i = 1; i < NUM_CITIES ; i++) {
       ax_add_test[i-1] = (int8_t)cities[optimalPath[i]].x;
       ay_add_test[i-1] = (int8_t)cities[optimalPath[i]].y;
    }
		
//    printf("%d\n", cities[optimalPath[NUM_CITIES - 1]].y);

//    printf("����·�����ȣ�%.2f\n", antLengths[bestAntIndex]);

//	    for (int i = 0; i < 6; i++) {
//			tft180_show_int(0,15*i,ax_add_test[i],3);
//			tft180_show_int(30,15*i,ay_add_test[i],3);
//			tft180_show_int(60,15*i,ax_add_test[i+6],3);
//			tft180_show_int(90,15*i,ay_add_test[i+6],3);
////        printf("%d ", optimalPath[i]);
//    }
//		
}
