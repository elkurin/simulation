#include <iostream>
#include <array>
#include <random>
#include <math.h>
#include <ostream>
#include <fstream>
#include <utility>
#include <iomanip>

using namespace std;

const int cell_max = 200;
const int init_cell_number = 10;//number of type of cell
const int time_end = 1000;
const double time_bunkai = 0.01;
const int run_time = 1;
const int init_each = 5;

const int N = 20;//number of node per cell
int cell_number = init_cell_number;

double aver_nut;
double nut_coef;

double box_con = 1;
double outside_nut; //mol
double box_init_size = 100000;
double box_size;
double box_size_prev;


double coef_decrease;
double total_size;

array<double, N> outside; //mol
array<double, N> go;

double debug_sum;
double debug_add = 0;

typedef struct
{
	double coef;
	double mol;//数
	double reversible;
	int catalyst;
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
Cell def;
pair<int, int> type_number;

double rd(void)
{
	double get;
	get = (double)(rand() % 1000) * 0.001 + 0.001;
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

double get_box_size(void)
{
	return box_init_size - get_total_size();
}

double decide_box_nut(double time)
{
	double get = aver_nut * 0.1;
	return get;
}

array<array<double, N>, init_cell_number> begin_coef;

void sum_ten_init(void)
{
	double sum = 0;
	array<double, N> keep;
	for (int i = 0; i < init_cell_number; i++) {
		for (int j = 0; j < N; j++) {
			keep.at(j) = rd();
			sum += keep.at(j);
		}
		for (int j = 0; j < N; j++) {
			begin_coef.at(i).at(j) = 10 * keep.at(j) / sum;
		}
		sum = 0;
	}
}

void one_init(void)
{
	for (int i = 0; i < init_cell_number; i++) {
		for (int j = 0; j < N; j++) {
			begin_coef.at(i).at(j) = 1;
		}
	}
}

void init(void)
{
	sum_ten_init();
	// one_init();
	for (int i = 0; i < init_cell_number; i++) {
		k.at(i) = def;
	}
	cell_number = init_cell_number;
	aver_nut = 1;
	outside_nut = (rand() % 10 + 1) * 0.1 * box_init_size;
	nut_coef = 1;
	coef_decrease = 0;

	for (int i = 0; i < N; i++) {
		outside.at(i) = (rand() % 10 + 1) * 0.1 * box_init_size;
		go.at(i) = 0;
	}
	for (int i = 0; i < cell_number; i++) {
		k.at(i).type = i;
		for (int j = 0; j < N; j++) {
			k.at(i).node.at(j).mol = rand() % 20 + 1;
			k.at(i).node.at(j).coef = begin_coef.at(i).at(j);
			k.at(i).node.at(j).reversible = 0;
			k.at(i).node.at(j).catalyst = (j - 2 + N) % N;
		}

		k.at(i).inside_nut = rand() % 20 + 1;
		k.at(i).size = get_size(k.at(i));
		k.at(i).init = k.at(i).size;
		k.at(i).init_last = k.at(i).node.at(N - 1).mol;
	}
	for (int i = 0; i < init_each; i++) {
		for (int j = 0; j < init_cell_number; j++) {
			k.at(init_cell_number * i + j) = k.at(j);
		}
	}
	cell_number = init_each * init_cell_number;
	total_size = get_total_size();
	box_size = get_box_size();
}

double prev_outside_nut_con;
double new_outside_nut_con;
array<double, N> new_outside_con;
array<double, N> prev_outside_con;

Cell internal(Cell p)
{
	array<double, N> new_con;
	array<double, N> prev_con;

	for (int i = 0; i < N; i++) {
		prev_con.at(i) = p.node.at(i).mol / p.size;
		new_con.at(i) = prev_con.at(i);
	}
	double prev_nut_con = p.inside_nut / p.size;
	double new_nut_con = prev_nut_con;

	new_nut_con += time_bunkai * nut_coef * pow(p.size, - 1.0 / 3.0) * (prev_outside_nut_con - prev_nut_con);
	new_outside_nut_con -= time_bunkai * nut_coef * pow(p.size, - 1.0 / 3.0) * (prev_outside_nut_con - prev_nut_con) * p.size / box_size;

	new_con.at(0) += time_bunkai * p.node.at(0).coef * new_nut_con;
	new_nut_con -= time_bunkai * p.node.at(0).coef * new_nut_con;
	for (int i = 1; i < N; i++) {
		new_con.at(i) += time_bunkai * p.node.at(i).coef * prev_con.at(i - 1) * prev_con.at(p.node.at(i).catalyst);
		new_con.at(i - 1) -= time_bunkai * p.node.at(i).coef * prev_con.at(i - 1) * prev_con.at(p.node.at(i).catalyst);
	}
	// if (p.type == 4) {
	// 	cout << new_nut_con - prev_nut_con << endl;
	// }

	new_nut_con += time_bunkai * p.node.at(0).reversible * prev_con.at(0);
	new_con.at(0) -= time_bunkai * p.node.at(0).reversible * prev_con.at(0);
	for (int i = 0; i < N - 1; i++) {
		new_con.at(i) += time_bunkai * p.node.at(i + 1).reversible * prev_con.at(i + 1);
		new_con.at(i + 1) -= time_bunkai * p.node.at(i + 1).reversible * prev_con.at(i + 1);
	}

	for (int i = 0; i < N; i++) {
		new_con.at(i) += time_bunkai * go.at(i) * (prev_outside_con.at(i) - prev_con.at(i));
		new_outside_con.at(i) += time_bunkai * go.at(i) * (prev_con.at(i) - prev_outside_con.at(i)) * p.size / box_size;
	}

	for (int i = 0; i < N; i++) {
		p.node.at(i).mol = new_con.at(i) * p.size;
	}
	p.inside_nut = new_nut_con * p.size;
	// cout << p.size << " " << get_size(p) << endl;
	double sum = new_nut_con;
	for (int i = 0; i < N; i++) {
		sum += new_con.at(i);
	}
	// cout << "sum = " << sum << endl;
	p.size = sum * p.size;
	// cout << p.size << " " << p.node.at(N - 1).mol << " ";

	// cout << p.node.at(1).mol << endl;
	return p;
}

pair<Cell, Cell> devide(Cell p)
{
	Cell q, r;
	q = p;
	r = p;

	q.inside_nut = 0.6 * p.inside_nut;
	r.inside_nut = p.inside_nut - q.inside_nut;

	for (int i = 0; i < N; i++) {
		q.node.at(i).mol = 0.6 * p.node.at(i).mol;
		r.node.at(i).mol = p.node.at(i).mol - q.node.at(i).mol;
	}

	q.size = get_size(q);
	r.size = get_size(r);

	pair<Cell, Cell> s = {q, r};
	return s;
}

void process(double t)
{
	box_con = decide_box_nut(t);
	prev_outside_nut_con = outside_nut / box_size;
	new_outside_nut_con = prev_outside_nut_con;
	for (int i = 0; i < N; i++) {
		prev_outside_con.at(i) = outside.at(i) / box_size;
		new_outside_con.at(i) = prev_outside_con.at(i);
	}

	for (int i = 0; i < cell_number; i++) {
		// cout << i << " ";
		k.at(i) = internal(k.at(i));
	}
	// cout << endl;

	for (int j = 0; j < cell_number; j++) {
		if (k.at(j).node.at(N - 1).mol / k.at(j).size > k.at(j).init_last / k.at(j).init * 2 || k.at(j).size > 10 * k.at(j).init) {
			cell_number++;

			pair<Cell, Cell> dev = devide(k.at(j));

			k.at(j) = dev.first;
			if (cell_number == cell_max) {
				int get;
				do {
					get = rand() % cell_number;
				} while(get == j);
				debug_add += k.at(get).size;
				cell_number--;
				k[get] = dev.second;
			} else {
				k.at(cell_number - 1) = dev.second;
			}
		} else if (k.at(j).node.at(N - 1).mol / k.at(j).size < k.at(j).init_last / k.at(j).init * 0.5) {
			debug_add += k.at(j).size;
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
		cout << setw(4) << a[i] << " ";
	}
	cout << endl;

	// new_outside_nut_con = prev_outside_nut_con + time_bunkai * nut_coef * (box_con - prev_outside_nut_con) * box_init_size; 
	outside_nut = new_outside_nut_con * box_size + time_bunkai * (box_con - prev_outside_nut_con) * box_init_size;
	// outside_nut = new_outside_nut_con * box_size;
	box_size = get_box_size();
	for (int i = 0; i < N; i++) {
		outside.at(i) = new_outside_con.at(i) * box_size;
	}

	for (int i = 0; i < N; i++) {
		// cout << outside.at(i) / box_size << " ";
	}
	// cout << endl;
}

int main(void)
{
	srand(1);
	def.type = - 1;
		
	for (int l = 0; l < run_time; l++) {
		init();
		for (double t = 1; t < time_end; t++) {
			process(t);
			debug_sum = get_total_size() + outside_nut;
			for (int i = 0; i < N; i++) {
				debug_sum += outside.at(i);
			}
			// cout << debug_sum + debug_add << endl;
		}
		for (int i = 0; i < init_cell_number; i++) {
			for (int j = 0; j < N; j++) {
				// cout << setw(4) << begin_coef.at(i).at(j) << " ";
			}
			// cout << endl;
		}
	}
	for (int i = 0; i < N; i++) {
		cout << begin_coef.at(3).at(i) << " ";
	}
	cout << endl;
	return 0;
}

