#include <iostream>
#include <math.h>
using namespace std;

double x = 1;
double dt = 0.001;
double t = 0;

int main(void)
{
	while(t < 1) {
		x += (3 * x + 2) * dt;
		cout << t << " " << x << endl; 
		t += dt;
	}
	return 0;
}


