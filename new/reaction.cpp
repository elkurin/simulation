#include <iostream>
#include <random>
#include "function.hpp"
#include "variable.hpp"

using namespace std;

const double time_bunkai = 0.01;
const double C = 0.1; //外のnutの濃度
double coef_decrease = 0.02;

//node[0]は外とバランスをとる養分
Cell internal_cell(Cell p)
{
	double prev_size = p.size;
	p.size = 0;
	for (int i = 0; i < N; i++) {
		p.prev_node[i] = p.node[i];
	}
	p.node[0] += time_bunkai * (C - p.node[0] / prev_size);
	for (int i = 1; i < N; i++) {
		p.node[i] -= coef_decrease * p.node[i];
		for (int j = 0; j < N; j++) {
			p.node[i] += time_bunkai * p.network[j][i].connected * p.prev_node[j] * p.node[p.network[j][i].catalyst];
		}
		p.size += p.node[i] * prev_size; 
	}

	for (int i = 0; i < N; i++) {
		p.node[i] *= prev_size / p.size;
	}
	return p;
}

void checker(Cell p)
{
	if (p.size >= 0.1 * p.init_size) {
		cout << "OK" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << i << " -> " << j << " = " << p.network[i][j].connected << " ";
				if (p.network[i][j].connected) cout << p.network[i][j].catalyst;
				cout << endl;
			}
		}
	} else {
		cout << "DEAD" << endl;
	}	
}
