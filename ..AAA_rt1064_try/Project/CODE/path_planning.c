/*遗传算法路径规划*/

#include "path_planning.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUM_CITIES 13    // 城市数量，算上起点1，1
#define NUM_ANTS 50    // 蚂蚁数量 
#define ALPHA 1.0			// α（信息素重要程度因子）
#define BETA 5.0         // β（启发式因子）
#define RHO 0.5          // ρ（信息素挥发系数）
#define Q 100            // Q（信息素增强强度）
#define MAX_ITERATIONS 100

extern jieshou_try_need jieshoushuju;

typedef struct {
    int x;    // 城市的x坐标
    int y;    // 城市的y坐标
} City;

	extern int8 x_add_test[12];
	extern int8 y_add_test[12];

int ax_add_test[12] = {5,10,5,12,17,33,6,22,13,29,5,33};   // 城市的x坐标数组，共有 NUM_CITIES 个元素
int ay_add_test[12] = {5,5,10,10,12,13,15,16,18,21,22,23};     // 城市的y坐标数组，共有 NUM_CITIES 个元素

double distances[NUM_CITIES][NUM_CITIES];      // 存储城市间距离的二维数组
double pheromones[NUM_CITIES][NUM_CITIES];     // 存储信息素浓度的二维数组

void initializeDistances(City cities[]) {
    // 根据城市之间的欧几里得距离计算距离矩阵
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            distances[i][j] = sqrt(dx * dx + dy * dy);
        }
    }
}

void initializePheromones() {
    // 初始化信息素矩阵，将其全部设置为一个初始浓度值
    double initialPheromone = 1.0 / ((double)NUM_CITIES * (double)distances[0][NUM_CITIES - 1]);

    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] = initialPheromone;
        }
    }
}

// 判断城市是否合法
int isValidCity(int city, int visited[]) {
    return (city >= 0 && city < NUM_CITIES && visited[city] == 0);
}

// 选择下一个要访问的城市
int selectNextCity(int ant, int currCity, int visited[]) {
    double total = 0.0;
    double probabilities[NUM_CITIES] = { 0.0 };

    // 根据信息素和启发式因子计算每个未访问城市的概率
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

    // 生成一个随机数，并根据概率选择下一个城市
    double random = (double)rand() / RAND_MAX * total;

    for (int i = 0; i < NUM_CITIES; i++) {
        if (probabilities[i] > random && isValidCity(i, visited)) {
            return i;
        }
    }

    return -1;
}

// 更新信息素矩阵
void updatePheromones(int antSolutions[][NUM_CITIES], double deltaPheromones[][NUM_CITIES]) {
    int globalBestAntIndex = -1;
    double globalBestLength = -1.0;

    // 计算每只蚂蚁的解，并更新信息素
    for (int ant = 0; ant < NUM_ANTS; ant++) {
        double solutionLength = 0.0;

        // 计算每个解的路径长度
        for (int i = 0; i < NUM_CITIES - 1; i++) {
            int city1 = antSolutions[ant][i];
            int city2 = antSolutions[ant][i + 1];
            solutionLength += distances[city1][city2];
        }
        solutionLength += distances[antSolutions[ant][NUM_CITIES - 1]][antSolutions[ant][0]];

        // 更新全局最优路径和长度
        if (globalBestLength == -1 || solutionLength < globalBestLength) {
            globalBestLength = solutionLength;
            globalBestAntIndex = ant;
        }

        // 更新信息素增加量
        for (int i = 0; i < NUM_CITIES - 1; i++) {
            int city1 = antSolutions[ant][i];
            int city2 = antSolutions[ant][i + 1];
            deltaPheromones[city1][city2] += Q / solutionLength;
            deltaPheromones[city2][city1] += Q / solutionLength;
        }
    }

    // 更新全局最优路径的信息素增加量
    for (int i = 0; i < NUM_CITIES - 1; i++) {
        int city1 = antSolutions[globalBestAntIndex][i];
        int city2 = antSolutions[globalBestAntIndex][i + 1];
        deltaPheromones[city1][city2] += Q / globalBestLength;
        deltaPheromones[city2][city1] += Q / globalBestLength;
    }

    // 更新全局信息素矩阵
    for (int i = 0; i < NUM_CITIES; i++) {
        for (int j = 0; j < NUM_CITIES; j++) {
            pheromones[i][j] *= (1 - RHO);
            pheromones[i][j] += deltaPheromones[i][j];
        }
    }
}

// 蚁群算法优化
int antColonyOptimization(City cities[], int optimalPath[], double antLengths[]) {
    initializeDistances(cities);
    initializePheromones();

    int antSolutions[NUM_ANTS][NUM_CITIES];

    // 迭代 MAX_ITERATIONS 次
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double deltaPheromones[NUM_CITIES][NUM_CITIES] = { 0.0 };

        // 每只蚂蚁构造一个解，并计算解的路径长度
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            int visited[NUM_CITIES] = { 0 };
            int currCity = 0;
            visited[currCity] = 1;
            antSolutions[ant][0] = currCity;

            // 根据概率选择下一个要访问的城市，直到访问完所有城市
            for (int i = 1; i < NUM_CITIES; i++) {
                int nextCity = selectNextCity(ant, currCity, visited);

                if (nextCity == -1) {
                    break;
                }

                antSolutions[ant][i] = nextCity;
                visited[nextCity] = 1;
                currCity = nextCity;
            }

            // 计算当前解的路径长度
            double solutionLength = 0.0;
            for (int i = 0; i < NUM_CITIES - 1; i++) {
                int city1 = antSolutions[ant][i];
                int city2 = antSolutions[ant][i + 1];
                solutionLength += distances[city1][city2];
            }
            solutionLength += distances[antSolutions[ant][NUM_CITIES - 1]][antSolutions[ant][0]];
            antLengths[ant] = solutionLength;
        }

        // 更新信息素矩阵
        updatePheromones(antSolutions, deltaPheromones);
    }

    int globalBestAntIndex = -1;
    double globalBestLength = -1.0;

    // 找到全局最优路径的蚂蚁
    for (int ant = 0; ant < NUM_ANTS; ant++) {
   // 更新全局最优路径和长度
    if (globalBestLength == -1 || antLengths[ant] < globalBestLength) {
        globalBestLength = antLengths[ant];
        globalBestAntIndex = ant;
			}
		}
		// 将全局最优路径保存到 optimalPath 数组中
		for (int i = 0; i < NUM_CITIES; i++) {
    optimalPath[i] = antSolutions[globalBestAntIndex][i];
	}
	return globalBestLength;
}

void path() {
	
    City cities[NUM_CITIES];//初始化城市坐标
    int optimalPath[NUM_CITIES];//储存最优路径的数组 
    double antLengths[NUM_ANTS];//储存每只蚂蚁的路径长度

    cities[0].x = 1;
    cities[0].y = 1;   //起始点

		
	
    for (int i = 1; i < NUM_CITIES; i++) {
        cities[i].x = ax_add_test[i-1];
        cities[i].y = ay_add_test[i-1];
    }

    int bestAntIndex = antColonyOptimization(cities, optimalPath, antLengths);

//    printf("最优路径：");
//    for (int i = 0; i < 4; i++) {
//			tft180_show_int(0,15*i,optimalPath[i],3);
//			tft180_show_int(30,15*i,optimalPath[i+4],3);
//			tft180_show_int(60,15*i,optimalPath[i+8],3);
//			tft180_show_int(90,15*i,optimalPath[i+12],3);
//        printf("%d ", optimalPath[i]);
//    }
//    printf("\n");

		//接收
		
//	printf("重新排列后的 ax_add_test:");
//    for (int i = 1; i < NUM_CITIES ; i++) {
//       jieshoushuju.X[i-1] = cities[optimalPath[i]].x;
//       jieshoushuju.Y[i-1] = cities[optimalPath[i]].y;
//    }
		
		    for (int i = 1; i < NUM_CITIES ; i++) {
       ax_add_test[i-1] = (int8_t)cities[optimalPath[i]].x;
       ay_add_test[i-1] = (int8_t)cities[optimalPath[i]].y;
    }
		
//    printf("%d\n", cities[optimalPath[NUM_CITIES - 1]].y);

//    printf("最优路径长度：%.2f\n", antLengths[bestAntIndex]);

//	    for (int i = 0; i < 6; i++) {
//			tft180_show_int(0,15*i,ax_add_test[i],3);
//			tft180_show_int(30,15*i,ay_add_test[i],3);
//			tft180_show_int(60,15*i,ax_add_test[i+6],3);
//			tft180_show_int(90,15*i,ay_add_test[i+6],3);
////        printf("%d ", optimalPath[i]);
//    }
//		
}
