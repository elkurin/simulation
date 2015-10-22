#include <iostream>
#include <array>
#include <random>

using namespace std;

const int node_max = 1000000;
const int init_node_number = 2;
const int time_end = 10000;
double time_bunkai = 0.001;

int node_number = init_node_number;

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

} Node;

array<node, node_max> k;

double get_random(void)
{
	random_device rd;
	double get = (double)rd() / 10000;
	return get;
}

void init(void)
{
	//one
	k.at(0).type = 0;

	k.at(0).a = 0.1;
	k.at(0).b = 0.1;
	k.at(0).c = 0.1;
	
	k.at(0).x = 1;
	k.at(0).y = 1;
	k.at(0).z = 1;
	
	k.at(0).prev_x = 0.1;
	k.at(0).prev_y = 0.1;
	k.at(0).prev_z = 0.1;
	
	//two
	k.at(1).type = 1;

	k.at(1).a = 0.1;
	k.at(1).b = 0.1;
	k.at(1).c = 0.1;
	
	k.at(1).x = 1;
	k.at(1).y = 1;
	k.at(1).z = 1;
	
	k.at(1).prev_x = 0.1;
	k.at(1).prev_y = 0.1;
	k.at(1).prev_z = 0.1;
}

Node internal(Node p)
{
	p.x = p.prev_x + time_bunkai * (p.a * p.x * p.z);
	p.y = p.prev_y + time_bunkai * (p.b * p.y * p.x);
	p.z = p.prev_z + time_bunkai * (p.c * p.z * p.y);

	p.prev_x = p.x;
	p.prev_y = p.y;
	p.prev_z = p.z;

	if (p.prev_x < 0) p.prev_x = 0;
	if (p.prev_y < 0) p.prev_y = 0;
	if (p.prev_z < 0) p.prev_z = 0;

	return p;
}

	
int main(void)
{
	init();


	for (int t = 0; t < time_end; t++) {
		proces();
	}

	return 0;
}

