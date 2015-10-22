#include <iostream>
#include <array>

using namespace std;

double time_bunkai = 0.001;
int time_end = 1000000;

int main(void)
{
	/*
	double a = 5;
	double b = 0.5;
	double c = 5;
	double d = 0.09;
	*/

	double a = 0.6;
	double b = 0.1;
	double c = 0.7;
	double d = 0.1;

	double x;
	double y;

	double prev_x = 10;
	double prev_y = 10;

	for (int t = 0; t < time_end; t++) {
		cout << prev_x << " " << prev_y << endl;

		x = prev_x + time_bunkai * prev_x * (a - b * prev_y);
		y = prev_y + time_bunkai * prev_y * (- c + d * prev_x);
		
		prev_x = x;
		prev_y = y;

		if (prev_x < 0) prev_x = 0;
		if (prev_y < 0) prev_y = 0;
	}

	return 0;
}
