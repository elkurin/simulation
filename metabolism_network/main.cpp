#include <iostream>
#include <array>
#include <math.h>

using namespace std;

const int type_number = 3;

double size;
int time_end = 100;

class Node
{
public :
	double x;						//concentration
	double catalyst;				//concentration xとともに次の物質を生成するための触媒
	double x_number;				//mol的な概念
	double catalyst_number;			//mol  
	double coef;
//	double prev_x;					//concentration
//	double prev_catalyst;			//concentration
	double coef_cat;				//catalystが増えるcoef
};

array<Node, 100> node;

void init(void)
{
	node.at(1) = {1, 1, 0, 0, 1, 1};
	node.at(2) = {0, 1, 0, 0, 2, 2};
	node.at(3) = {0, 0, 0, 0, 1, 2};
}

int main(void)
{
	init();

	for (int t = 0; t < time_end; t++) {
		node.at(0) = node.at(type_number);
		node.at(type_number + 1) = node.at(1);
		for (int i = 1; i <= type_number; i++) {
			node.at(i).x_number += node.at(i - 1).coef * node.at(i - 1).x * node.at(i - 1).catalyst - node.at(i + 1).coef * node.at(i + 1).x * node.at(i + 1).catalyst;
			node.at(i).catalyst_number += node.at(i).coef_cat * node.at(i).x;
		}

		double sum = 0;
		for (int i = 1; i <= type_number; i++) {
			sum += node.at(i).x_number + node.at(i).catalyst_number;
		}

		size = sum;
		for (int i = 1; i <= type_number; i++) {
			node.at(i).x = node.at(i).x_number / sum;
			node.at(i).catalyst = node.at(i).catalyst_number / sum;
		}
		cout << t << " " << size;
		for (int i = 1; i <= type_number; i++) {
			cout << " " << node.at(i).x << " " << node.at(i).catalyst;
		}
		cout << endl;
	}
	return 0;
}

		
			

