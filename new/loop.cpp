#include <iostream>
#include <array>
#include <random>
#include <math.h>
#include <ostream>
#include <fstream>
#include <utility>

using namespace std;

namespace {
	ofstream take_log("data_check_loop.log");
}

const int cell_max = 1000;
const int init_cell_number = 10;
const int time_end = 10000;
double time_bunkai = 0.01;
const int run_time = 1;

const int N = 3;
int cell_number = init_cell_number;

double nutorition;
double aver_nut;
double nut_coef;

int a[init_cell_number];

double coef_decrease;

double total_size;

typedef struct
{
	double coef;
	double mol;
} Node;

typedef struct 
{
	int type;

	double inside_nut;
	double prev_nut;

	array<Node, N> node;

	double size;
	double init;
	double init_last;
} Cell;

array<Cell, cell_max> k;

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

double get_size(Cell p)
{
	p.size = p.inside_nut;
	for (int i = 0; i < N; i++) {
		p.size += p.node.at(i).mol;
	}
	return p.size;
}

double get_total_size(void)
{
	double q;
	for (int j = 0; j < cell_number; j++) {
		q += k.at(j).size;
	}
	return q;
}

double decide_nut(double time)
{
	double get = aver_nut * (1 + 0.9 * sin(time / 1000));
//	double get = aver_nut * 1;
	return get;
}

double begina[init_cell_number], beginb[init_cell_number], beginc[init_cell_number];

array<<array<double, N>, init_cell_number> begin;

void first_init(void)
{
	for (int i = 0; i < init_cell_number; i++) {
		for (int j = 0; j < N; j++) {
			cin >> begin.at(i).at(j);
		}
	}
}

void init(void)
{
	for (int i = 0; i < init_cell_number; i++) {
		k.at(i) = def;
	}
	cell_number = init_cell_number;
	aver_nut = 0.25;
	nut_coef = 1;
	coef_decrease = 0.01;

	for (int i = 0; i < cell_number; i++) {
		k.at(i).type = i;
		for (int j = 0; j < N; j++) {
			k.at(i).node.at(j).mol = 1;
			k.at(i).node.at(j).coef = begin.at(i).at(j);
		}
		k.at(i).inside_nut = 1;
		k.at(i).size = get_size(k.at(i));
		k.at(i).init = k.at(i).size;
		k.at(i).init_last = k.at(i).node.at(N).mol;
	}
	for (int i = 1; i < 5; i++) {
		for (int j = 0; j < init_cell_number; j++) {
			k.at(init_cell_number * i + j) = k.at(j);
		}
	}
	cell_number = 5 * cell_number;
}
/*
void random_init(void)
{
	aver_nut = 0.25;
	nut_coef = 1;
	coef_decrease = 0.01;
	for (int i = 0; i < cell_number; i++) {
		k.at(i).type = i;
		k.at(i).x = 0.1;
		k.at(i).y = 0.1;
		k.at(i).z = 0.1;
		k.at(i).inside_nut = 0.1;
		k.at(i).size = 0.3;
		k.at(i).init_z = 0.1;

		while(1) {
			k.at(i).a = rd();
			k.at(i).b = rd();
			k.at(i).c = 1 - k.at(i).a - k.at(i).b;
			if (k.at(i).c > 0) break;
		}
	}
}
*/
Cell internal(Cell p, double outside_nut)
{
	array<double, N> new_con;
	array<double, N> prev_con;
	for (int i = 0; i < N; i++) {
		prev_con.at(i) = p.node.at(i).mol / p.size;
	}
	double nut_con = p.inside_nut / p.size;

	double nut_new = nut_con + nut_coef * pow(p.size, 2 / 3) * (outside_nut - nut_con);
	new_con.at(0) = prev_con.at(i) + time_bunkai * p.node.at(0).coef * nut_con;
	for (int i = 1; i < N; i++) {
		new_con.at(i) = prev_con.at(i) + time_bunkai * p.node.at(i).coef * new_con.at(i - 1);
	}

	nut_new -= x_new - x_con;
	for (int i = 0; i < N - 1; i++) {
		new_con.at(i) -= new_con.at(i + 1) - prev_con.at(i + 1);
	}
	new_con.at(N - 1) -= time_bunkei * coef_decrease * prev_con.at(N - 1);

	double sum = nut_new;
	for (int i = 0; i < N; i++) {
		sum += new_con.at(i);
	}
	p.size = sum * p.size;

	for (int i = 0; i < N; i++) {
		p.node.at(i).mol = new_con.at(i) * p.size;
	}
	p.inside_nut = nut_new * p.size;

//	cout << p.size << " " << p.x << " " << p.y << " " << p.z << " " << p.inside_nut << endl;

	return p;
}

pair<Cell, Cell> devide(Cell p)
{
	Node q, r;

	q = p;
	r = p;
	
	q.inside_nut = 0.5 * get_rand_normal(p.inside_nut);
	r.inside_nut = p.inside_nut - q.inside_nut;

	for (int i = 0; i < N; i++) {
		q.node.at(i).mol = get_rand_normal(p.node.at(i).mol) * 0.5;
		r.node.at(i).mol = p.node.at(i).mol - q.node.at(i).mol;
	}

	q.size = get_size(q);
	r.size = get_size(r);

	/*
	q.a = get_rand_normal(p.a);
	q.b = get_rand_normal(p.b);
	q.c = get_rand_normal(p.c);

	r.a = get_rand_normal(p.a);
	r.b = get_rand_normal(p.b);
	r.c = get_rand_normal(p.c);
	*/

	pair<Cell, Cell> s = {q, r};
	return s;
}

void process(double t)
{
	//nodeの内部変化
	nutorition = decide_nut(t);
	//give_nut(nutorition);

//	cout << t << " ";
//	take_log << nutorition << " ";
	for (int j = 0; j < cell_number; j++) {
		k.at(j) = internal(k.at(j), nutorition);
	}
	//分裂
	for (int j = 0; j < cell_number; j++) {
		if (k.at(j).node(N).mol > k.at(j).init_last * 2 || k.at(j).size > 10 * k.at(j).init) {//zが2倍またはsizeが10倍になると分裂
			cell_number++;

			pair<Cell, Cell> dev;
			dev = devide(k.at(j));
			
			k.at(j) = dev.first;
			if (cell_number == node_max) { //node_maxを越えると外に流れ出る
				int get;
				do {
					get  = rdom() % cell_number;
				} while (get == j);
				k.at(get) = dev.second;
				cell_number--;
			} else {
				k.at(cell_number - 1) = dev.second;
			}
		} else if (k.at(j).node(N).mol < k.at(j).init_last * 0.5) { //zが1/2になると消滅
			k.at(j) = k.at(cell_number - 1);
			cell_number--;
		}
	}
	
	for (int i = 0; i < init_cell_number; i++) {
		a[i] = 0;
	}
	for (int j = 0; j < cell_number; j++) {
		a[k.at(j).type]++;
	}
	for (int i = 0; i < init_cell_number; i++) {
		cout << a[i] << " ";
//		take_log << a[i] << " ";
	}
	cout << endl;
//	take_log << endl;
	total_size = get_total_size();
	for (int i = 0; i < 3; i++) {
//		cout << k.at(i).x << " " << k.at(i).y << " " << k.at(i).z << " ";
	}
//	cout << endl;
}
	
int main(void)
{
	Node def;
	def.type = -1;
	first_init();
	int aver[init_cell_number][time_end];
	for (int i = 0; i < init_cell_number; i++) {
		for (int j = 0; j < time_end; j++) {
			aver[i][j] = 0;
		}
	}
	for (int l = 0; l < run_time; l++) {
		init();
		for (int i = 0; i < init_cell_number; i++) {
			cout << k.at(i).a << " " << k.at(i).b << " " << k.at(i).c << endl;
			take_log << k.at(i).a << " " << k.at(i).b << " " << k.at(i).c << endl;
		}
		for (double t = 1; t < time_end; t++) {
			process(t);
			for (int i = 0; i < init_cell_number; i++) {
				aver[i][(unsigned int)t] += a[i];
			}
		}
		//int check = 1;
//		for (int i = 0; i < cell_number; i++) {
//			if (k.at(i).x + k.at(i).y + k.at(i).z == 0) check = 0;
//		}
		//if (check == 1) cout << "OK" << endl;
		//else cout << "Not OK" << endl;
//		cout << k.at(0).x << " " << k.at(0).y << " " << k.at(0).z << " " << k.at(0).inside_nut << endl;
	//	if (l % 2 == 0) {
		//	cout << type_number.first << " " << type_number.second << endl;
		//	take_log << type_number.first << " " << type_number.second << endl;
	//	}
	}
	for (int t = 0; t < time_end; t++) {
		for (int i = 0; i < init_cell_number; i++) {
			cout << aver[i][t] / run_time << " ";
			take_log << aver[i][t] / run_time << " ";
		}
		cout << endl;
		take_log << endl;
	}
//	cout << endl;
//	take_log << endl;
	return 0;
}

