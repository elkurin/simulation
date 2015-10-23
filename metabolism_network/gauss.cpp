#include <iostream>
#include <random>
#include <math.h>
#include <ostream>
#include <fstream>

using namespace std;

namespace {
	ofstream take_log("rand_normal_test.log");
}

double rd(void)
{
	random_device rdom;
	double get;
	get = (double)(rdom() % 1000) * 0.001;
	return get;
}

double rand_normal(double mu, double sigma)
{
	double z = sqrt(- 2.0 * log(rd())) * sin(2.0 * M_PI * rd());
	return mu + sigma * z;
}

int main(void)
{
	for (int t = 0; t < 1000; t++) {
		double get = rand_normal(0, 1);
		cout << t << " " << get << endl;
		take_log << t << " " << get << endl;
	}

	return 0;
}
