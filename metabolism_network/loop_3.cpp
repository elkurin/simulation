#include <iostream>
#include <array>
#include <random>
#include <math.h>
#include <ostream>
#include <fstream>
#include <utility>

using namespace std;

namespace {
	ofstream take_log("data_metabolism_typenumber_loop.log");
}

const int node_max = 100;
const int init_node_number = 2;
const int time_end = 10000;
double time_bunkai = 0.001;

int node_number = init_node_number;

double nutorition;
double aver_nut;
double nut_coef;

double nut_C = 0.1;

double total_size;

typedef struct 
{
	int type;

	double a;
	double b;
	double c;

	double x;
	double prev_x;
	double y;
	double prev_y;
	double z;
	double prev_z;

	double size;
	double init;
	double init_z;

	double get_nut;
} Node;

array<Node, node_max> k;

pair<int, int> type_number; //それぞれのtypeのnodeの数

random_device rdom;

double rd(void)
{
	double get;
	get = (double)(rdom() % 1000) * 0.001 + 0.001;
	return get;
}

double rand_normal(double mu, double sigma)
{
	double z = sqrt(- 2.0 * log(rd())) * sin(2.0 * M_PI * rd());
	return mu + sigma * z;
}

double get_rand_normal(double size)
{
	double get;
	get = rand_normal(size, size * 0.1);
	return get;
}

double get_size(Node p)
{
	p.size = p.x + p.y + p.z;
	return p.size;
}

double get_total_size(void)
{
	double q;
	for (int j = 0; j < node_number; j++) {
		q += k.at(j).size;
	}
	return q;
}

double decide_nut(double time)
{
	double get = aver_nut * (1 + 0.9 * sin(time / 360));
	return get;
}

void init(void)
{
	//nutorition
	aver_nut = 1;
//	nut_coef = 1;

	//one
	k.at(0).type = 0;

	k.at(0).a = 0.1;
	k.at(0).b = 0.1;
	k.at(0).c = 0.1;
	
	k.at(0).x = 0.1;
	k.at(0).y = 0.1;
	k.at(0).z = 0.1;
	
	k.at(0).prev_x = 0;
	k.at(0).prev_y = 0;
	k.at(0).prev_z = 0;
	
	k.at(0).size = get_size(k.at(0));
	k.at(0).init = k.at(0).size;
	k.at(0).init_z = k.at(0).z;

	k.at(0).get_nut = 0;
			
	//two
	k.at(1).type = 1;

	k.at(1).a = 1;
	k.at(1).b = 1;
	k.at(1).c = 1;
	
	k.at(1).x = 0.1;
	k.at(1).y = 0.1;
	k.at(1).z = 0.1;
	
	k.at(1).prev_x = 0;
	k.at(1).prev_y = 0;
	k.at(1).prev_z = 0;

	k.at(1).size = get_size(k.at(1));
	k.at(1).init = k.at(1).size;
	k.at(1).init_z = k.at(1).z;

	k.at(1).get_nut = 0;

	total_size = get_total_size();

	node_number = init_node_number;
}

void give_nut(double nut)
{
	double sum = 0;
	for (int i = 0; i < node_number; i++) {
		sum += pow(k.at(i).size, 2/3);
	}
	for (int i = 0; i < node_number; i++) {
		k.at(i).get_nut = nut * pow(k.at(i).size, 2/3) / sum;
	}
}

Node internal(Node p)
{
	p.prev_x = p.x;
	p.prev_y = p.y;
	p.prev_z = p.z;
	
	array<double, 3> prop;
	prop.at(0) = p.prev_x / p.size + time_bunkai * (p.a * (nut_C - p.prev_x / p.size));
	prop.at(1) = p.prev_y / p.size + time_bunkai * (p.b * p.prev_y * p.prev_x / (p.size * p.size));
	prop.at(2) = p.prev_z / p.size + time_bunkai * (p.c * p.prev_z * p.prev_y / (p.size * p.size));

	p.size += p.size * time_bunkai * (p.a * (nut_C - p.prev_x / p.size));
	double sum = prop.at(0) + prop.at(1) + prop.at(2);
	
	p.x = p.size * prop.at(0) / sum;
	p.y = p.size * prop.at(1) / sum;
	p.z = p.size * prop.at(2) / sum;

	if (p.x < 0) p.x = 0;
	if (p.y < 0) p.y = 0;
	if (p.z < 0) p.z = 0;

	p.size = get_size(p);

	return p;
}

pair<Node, Node> devide(Node p)
{
	Node q, r;

	q = p;
	r = p;
	
	q.x = get_rand_normal(p.x);
	r.x = p.x - q.x;

	q.y = get_rand_normal(p.y);
	r.y = p.y - q.y;

	q.z = get_rand_normal(p.z);
	r.z = p.z - q.z;

	q.size = get_size(q);
	r.size = get_size(r);

	q.init = q.size;
	r.init = r.size;

	q.a = get_rand_normal(p.a);
	q.b = get_rand_normal(p.b);
	q.c = get_rand_normal(p.c);

	r.a = get_rand_normal(p.a);
	r.b = get_rand_normal(p.b);
	r.c = get_rand_normal(p.c);

	pair<Node, Node> s = {q, r};
	return s;
}

void process(double t)
{
	//nodeの内部変化
	nutorition = decide_nut(t);
	//give_nut(nutorition);

	for (int j = 0; j < node_number; j++) {
		k.at(j) = internal(k.at(j));
	}
	//分裂
	for (int j = 0; j < node_number; j++) {
		if (k.at(j).z > k.at(j).init_z * 2) {
			node_number++;

			pair<Node, Node> dev;
			dev = devide(k.at(j));
			
			k.at(j) = dev.first;
			if (node_number == node_max) { //node_maxを越えると外に流れ出る
				node_number--;
				int get;
				do {
					get  = rdom() % node_number;
				} while (get == j);
				k.at(get) = dev.second;
			} else {
				k.at(node_number - 1) = dev.second;
			}
		} else if (k.at(j).z < k.at(j).init_z * 0.5) { //zが1/2になると消滅
			k.at(j) = k.at(node_number - 1);
			node_number--;
		}
	}
	
	type_number.first = 0;
	type_number.second = 0;

	for (int j = 0; j < node_number; j++) {
		if (k.at(j).type == 0) {
			type_number.first++;
		} else {
			type_number.second++;
		}
	}
//	cout << t << " " << type_number.first << " " << type_number.second << endl;
//	take_log << t << " " << type_number.first << " " << type_number.second << endl;
	total_size = get_total_size();
}
	
int main(void)
{
	for (int l = 0; l < 100; l++) {
		init();
		for (double t = 0; t < time_end; t++) {
			process(t);
		}
		//int check = 1;
		for (int i = 0; i < node_number; i++) {
			if (k.at(i).x + k.at(i).y + k.at(i).z == 0) check = 0;
		}
		//if (check == 1) cout << "OK" << endl;
		//else cout << "Not OK" << endl;

		if (l % 2 == 0) {
		//	cout << type_number.first << " " << type_number.second << endl;
		//	take_log << type_number.first << " " << type_number.second << endl;
		}
	}
	return 0;
}

