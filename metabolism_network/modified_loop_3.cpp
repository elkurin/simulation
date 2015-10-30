#include <iostream>
#include <array>
#include <random>
#include <math.h>
#include <ostream>
#include <fstream>
#include <utility>

using namespace std;

namespace {
	ofstream take_log("data_modified_metabolism_typenumber_loop.log");
}

const int node_max = 100;
const int init_node_number = 10;
const int time_end = 3000;
double time_bunkai = 0.01;

int node_number = init_node_number;

double nutorition;
double aver_nut;
double nut_coef;

double coef_decrease;

double total_size;

typedef struct 
{
	int type;

	double inside_nut;
	double prev_nut;

	double a;
	double b;
	double c;

	double x;		//濃度
	double prev_x;
	double y;
	double prev_y;
	double z;
	double prev_z;

	double size;
	double init;
	double init_z;
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
//	double get = aver_nut * (1 + 0.9 * sin(time / 360));
	double get = aver_nut * 2;
	return get;
}

void init(void)
{
	//nutorition
	aver_nut = 0.1;
	nut_coef = 1;

	//one
	k.at(0).type = 0;

	k.at(0).inside_nut = 0.1;
	k.at(0).a = 0.1;
	k.at(0).b = 0.1;
	k.at(0).c = 0.1;
	
	k.at(0).x = 0.1;
	k.at(0).y = 0.1;
	k.at(0).z = 0.1;
	
	k.at(0).prev_nut = 0;
	k.at(0).prev_x = 0;
	k.at(0).prev_y = 0;
	k.at(0).prev_z = 0;
	
	k.at(0).size = get_size(k.at(0));
	k.at(0).init = k.at(0).size;
	k.at(0).init_z = k.at(0).z;

	//two
	k.at(1).type = 1;

	k.at(1).inside_nut = 0.1;
	k.at(1).a = 0.5;
	k.at(1).b = 0.5;
	k.at(1).c = 0.5;
	
	k.at(1).x = 0.1;
	k.at(1).y = 0.1;
	k.at(1).z = 0.1;
	
	k.at(1).prev_nut = 0;
	k.at(1).prev_x = 0;
	k.at(1).prev_y = 0;
	k.at(1).prev_z = 0;

	k.at(1).size = get_size(k.at(1));
	k.at(1).init = k.at(1).size;
	k.at(1).init_z = k.at(1).z;

	total_size = get_total_size();

	node_number = init_node_number;

	for (int i = 2; i < 10; i++) {
		if (i % 2 == 0) {
			k.at(i) = k.at(0);
		} else {
			k.at(i) = k.at(1);
		}
	}

	coef_decrease = 0.02;
}

Node internal(Node p, double outside_nut)
{
	p.prev_nut = p.inside_nut;
	p.prev_x = p.x;
	p.prev_y = p.y;
	p.prev_z = p.z;
	
	p.inside_nut += nut_coef * pow(p.size, 2 / 3) * (outside_nut - p.prev_nut);
	p.x = p.prev_x + time_bunkai * (p.a * p.inside_nut);
	p.y = p.prev_y + time_bunkai * (p.b * p.prev_x * p.prev_z);
	p.z = p.prev_z + time_bunkai * (p.c * p.prev_y * p.prev_x) - time_bunkai * coef_decrease * p.prev_z;

	p.inside_nut -= p.x - p.prev_x;
	p.x -= p.y - p.prev_y;
	p.y -= p.z - p.prev_z;

	double prev_size = p.size;
	p.size += (p.inside_nut - p.prev_nut) * prev_size;

	if (p.size == 0) {
		p.x = 0;
		p.y = 0;
		p.z = 0;
	} else {
		p.x *= prev_size / p.size;
		p.y *= prev_size / p.size;
		p.z *= prev_size / p.size;
		p.inside_nut *= prev_size / p.size;
	}
	if (p.x < 0) p.x = 0;
	if (p.y < 0) p.y = 0;
	if (p.z < 0) p.z = 0;
	if (p.inside_nut < 0) p.inside_nut = 0;
	//cout << p.x << " " << p.y << " " << p.z << " " << p.inside_nut << endl;

	return p;
}

pair<Node, Node> devide(Node p)
{
	Node q, r;

	q = p;
	r = p;
	
	q.inside_nut = get_rand_normal(p.inside_nut);
	r.inside_nut = p.inside_nut - q.inside_nut;

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
		k.at(j) = internal(k.at(j), nutorition);
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
	cout << t << " " << type_number.first << " " << type_number.second << endl;
	take_log << t << " " << type_number.first << " " << type_number.second << endl;
	total_size = get_total_size();
}
	
int main(void)
{
	for (int l = 0; l < 1; l++) {
		init();
		for (double t = 1; t < time_end; t++) {
			process(t);
		}
		//int check = 1;
//		for (int i = 0; i < node_number; i++) {
//			if (k.at(i).x + k.at(i).y + k.at(i).z == 0) check = 0;
//		}
		//if (check == 1) cout << "OK" << endl;
		//else cout << "Not OK" << endl;
		cout << k.at(0).x << " " << k.at(0).y << " " << k.at(0).z << " " << k.at(0).inside_nut << endl;
	//	if (l % 2 == 0) {
		//	cout << type_number.first << " " << type_number.second << endl;
		//	take_log << type_number.first << " " << type_number.second << endl;
	//	}
	}
	return 0;
}

