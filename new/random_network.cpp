#include <iostream>
#include <random>
#include "variable.hpp"
#include "function.hpp"

using namespace std;

const double possiblity = 0.5;
const int time_end = 1000;

Cell cell[M];

random_device rd;

double rd0_1(void)
{
	double get = (double)(rd() % 1000) * 0.001;
	return get;
}

void make_random_network(void)
{
	for (int k = 0; k < M; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 1; j < N; j++) {
				double get = rd0_1();
				if (get < possiblity) {
					cell[k].network[i][j].connected = 1;
					int ran = rd() % (N - 3);
					if (ran != i && ran != j) {
						cell[k].network[i][j].catalyst = ran + 1;
					} else if (ran == i) {
						cell[k].network[i][j].catalyst = N - 2;
					} else {
						cell[k].network[i][j].catalyst = N - 1;
					}
				} else {
					cell[k].network[i][j].connected = 0;
				}
			}
		}
	}
}

void init(void)
{
	for (int i = 0; i < M; i++) {
		cell[i].size = 0;
		for (int j = 0; j < N; j++) {
			cell[i].node[j] = 1;
			cell[i].size += cell[i].node[j];
		}
		cell[i].init_size = cell[i].size;
	}
}


int main(void)
{
	for (int type = 0; type < 100000; type++) {
		init();
		//make_random_network();
		for (int t = 0; t < time_end; t++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					for(int k = 0; k < M; k++) {
						cell[k].network[i][j].connected = 1;
					}
				}
			}
			for (int i = 0; i < M; i++) {
				cell[i] = internal_cell(cell[i]);
			}
		}
		for (int i = 0; i < M; i++) {
			checker(cell[i]);
		}
	}
	return 0;
}


