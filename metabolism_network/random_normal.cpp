#include <iostream>
#include <random>

using namespace std;

random_device rdom;

double rd(void)
{
	double get;
	get = (double)(rdom() % 1000) * 0.001;
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
