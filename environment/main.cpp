#include <iostream>
#include <array>
#include <math.h>

using namespace std;

int i, j;
const int max_x = 1000, max_i = 1000, max_j = 1000; //max_xがノード数
double x[max_x];
int number;
double theta;
double ref;
double kp;
double next_x[max_x];
double t = 0;
int time_end;

class param
{
	public:
	array<int, max_x> J;
	double fitness;
	double proportion;
	double deathrate;
	double current_x;
	double next_x;
	int nutorition_type;
}

array<param, max_x> node;

void decide_param(void)
{

}

int main(void)
{
	for (t = 1; t <= time_end; t++) {
		cout << t / 100 << " ";
		for (i = 0; i < number; i++) {
			double sum = 0;
			for (j = 0; j < number; j++) {
				sum += J[i][j] * x[j] - theta;
			}
			double p = 1.0 / (1.0 + exp(-sum));
			next_x[i] = x[i] + 0.01 / kp * p - 0.01 * ref * x[i];
			if (next_x[i] < 0) next_x[i] = 0;
			cout << next_x[i] << " ";
		}
		cout << endl;
		for (i = 0; i < number; i++) {
			x[i] = next_x[i];
		}
	}

