#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <random>
#include <math.h>

using namespace std;

const int node_number = 100;
int type_number = 2;
const int init_type_number = 2;

const int mutation_fre = 10000;

int time_end = 100000;
double time_bai = 1;

//何回計測するか
int time_number = 20;

typedef struct
{
	double fitness;
	double proportion;
	int type;
	int life;
	int life_expectancy;
	double death;
	double death_weight;
	double eaten;
} Node;

Node node[node_number];
Node node_defo[1000];

array<int, 1000> type_survive;

void init_node_defo(void)
{
	node_defo[0] = {100, 0, 0, 0, 3, 0, 1, 0};
	node_defo[1] = {101, 0, 1, 0, 3, 0, 1, 0.06};
//	node_defo[2] = {102, 0, 2};
}
/*
void init_rand(void)
{
	srand((unsigned int)time(NULL));
}
*/
double make_random(void)
{
	random_device rd;
	int o = rd() % 10000;
	double p = (double)o * 0.0001;
	//cout << "rand = " << p << endl;
	return p;
}

int decide_growth(void)
{
	double get = make_random();
	double sum = 0;
	int new_node;
	for (int i = 0; i < node_number; i++) {
		sum += node[i].proportion;
		if (get < sum) {
			new_node = i;
			break;
		}
	}
	//cout << "new_node = " << new_node << endl;
	return new_node;
}

int decide_death_random(void)
{
	random_device rd;
	int get = rd() % node_number;
	int death_node;
	for (int i = 0; i < node_number; i++) {
		if (get == i) {
			death_node = i;
			break;
		}
	}
	//cout << "death_node = " << death_node << endl;
	return death_node;
}

int decide_death_le(void)
{
	double get = make_random();
	int death_node = -1;
	double sum = 0;
	for (int i = 0; i < node_number; i++) {
		/*
		if (node[i].life > 2 * node[i].life_expectancy) {
			node[i].death_weight = 3;
		} else if (node[i].life > node[i].life_expectancy) {
			node[i].death_weight = 2;
		} else {
			node[i].death_weight = 1;
		}
		*/
		node[i].death_weight = exp((double)node[i].life / (double)node[i].life_expectancy + node[i].eaten * type_survive.at(0));
		sum += node[i].death_weight;
//		cout << "sum = " << sum << endl;

	}
	double weight_sum = 0;
	for (int i = 0; i < node_number; i++) {
		node[i].death = node[i].death_weight / sum;
		weight_sum += node[i].death;
		if (weight_sum > get) {
//			cout << "random = " << get << endl;
			death_node = i;
			break;
		}
	}
//	cout << sum << " " << node[0].death << " " ;
//	cout << "death_node = " << death_node << endl;
	return death_node;
}
			

void check_proportion(void)
{
	double sum = 0;
	for (int i = 0; i < node_number; i++) {
		sum += node[i].fitness;
	}
	for (int i = 0; i < node_number; i++) {
		node[i].proportion = node[i].fitness / sum;
		//cout << "node[" << i << "].proprotion = " << node[i].proportion << " ";
	}
	//cout << endl;
}
/*
void 0_eat_1(void)
{
	if (type_survaive.at(0) > type_survive.at(1)) {

	} else {
		while(1) {
			for (int i = 0; i < node_number; i++) { 
				if (node[i].type == 1) 
*/
array<int, 1000> number_of_type;
void decide_type_random(void)
{
	for (int i = 0; i < node_number; i++) {
		int p = rand() % type_number;
		node[i].type = p;
		for (int j = 0; j < type_number; j++) {
			if (p == j) {
				node[i] = node_defo[j];
				number_of_type.at(j)++;
				break;
			}
		}
	}
//	cout << "0 ";
	for (int count = 0; count < type_number; count++) {
//		cout << number_of_type.at(count) << " ";
	}
//	cout << endl;
}

void mutation(int q)
{
	random_device rd;
	int p = rd() % mutation_fre;
	if (p == 0) {
		type_number++;
		node[q] = {double(rd() % 300), 0, type_number, 0, int(rd() % 40), 0, 1, 0};
	}
}

void main_(void)
{
	init_node_defo();
	decide_type_random();
	/*
	node[0].type = 0;
	node[1].type = 1;
	node[2].type = 0;
	node[3].type = 1;
	node[4].type = 0;
	for (int i = 0; i < node_number; i++) {
		for (int j = 0; j < type_number; j++) {
			if (node[i].type == j) {
				node[i] = node_defo[j];
//				cout << "fitnes is " << node[i].fitness << endl;
				break;
			}
		}
	}
	*/
	double t = 0;
//	cout << t << " " << "3 2" << endl;
	for (t = 1; t <= time_end; t++) {
//		cout << t * time_bai << " ";
		check_proportion();
		int get = decide_death_random();
		node[get] = node[decide_growth()];
		mutation(get);
		node[get].life = 0;
//		cout << decide_death() << " " << decide_growth() << endl;
//		cout << "node_number = " << node_number << " type_number = " << type_number << endl;
		for (int count = 0; count < type_number; count++) {
			type_survive.at(count) = 0;
			for (int i = 0; i < node_number; i++) {
				if (node[i].type == count) {
					type_survive.at(count)++;
				}
			}
//			cout << type_survive.at(count) << " ";
		}
//		cout << endl;
	}
	for (int count = 0; count < type_number; count++) {
		cout << type_survive.at(count) << " ";
	}
	cout << endl;
}

int main(void)
{
	int win[type_number];
	for (int i = 0; i < type_number; i++) {
		win[i] = 0;
	}
	for (int times = 0; times < time_number; times++) {
		for (int i = 0; i < node_number; i++) {
			node[i].life++;
		}
		main_();
//		cout << "life = " << node[0].life << endl;
		for (int i = 0; i < type_number; i++) {
			if (type_survive.at(i) == node_number) {
				win[i]++;
				cout << node[i].fitness << " " << node[i].life_expectancy << endl;
			}
		}
		type_number = init_type_number;
	}
	for (int i = 0; i < type_number; i++) {
		cout << win[i] << " ";
	}
	cout << endl;	
	return 0;
}
