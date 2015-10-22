#include <iostream>
#include "function.h"
#include "variable.h"

using namespace std;

double time_bunkai;
int time_end;
double t;
double x;

int main(void)
{
	need();
	cout << "Input the initial x -> ";
	cin >> x;
	cout << "Input the smallest time -> ";
	cin >> time_bunkai;
	while (time_bunkai <= 0) {
		cout << "Invalid. Try again -> ";
		cin >> time_bunkai;
	}
	cout << "Input the time to end [time * time_bunkai] -> ";
	cin >> time_end;

	t = 0;
	cout << t << " " << x << endl;
	while (t < time_end) {
		t++;
		x = equation_runge_kutta(x, time_bunkai);
		cout << t * time_bunkai << " " << x << endl;
	}
	
	return 0;
}
