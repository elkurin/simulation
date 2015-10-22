#include <iostream>

using namespace std;

int time_end;
double t;
double x;
double time_bunkai;

double equation(double t, double x)
{
	return 3 * x;
}

int main(void)
{
	cout << "Equation is" << endl;
	cout << "Input the time to end -> ";
	cin >> time_end;
	cout << "Input the smallest time -> ";
	cin >> time_bunkai; 
	cout << "Input the initial x -> ";
	cin >> x;

	t = 0;

	cout << t * time_bunkai << " " << x << endl;

	for (t = 1; t <= time_end; t++) {
		double k1 = equation(t, x);
		double k2 = equation(t + time_bunkai / 2, x + k1 * time_bunkai / 2);
		double k3 = equation(t + time_bunkai / 2, x + k2 * time_bunkai / 2);
		double k4 = equation(t + time_bunkai, x + k3 * time_bunkai); 
		x += time_bunkai / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
		cout << t * time_bunkai << " " << x << endl;
	}
	return 0;
}
