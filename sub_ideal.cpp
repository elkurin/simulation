#include <iostream>
#include <array>

using namespace std;

int time_end = 100000;
double time_bunkai = 0.001;

int main(void)
{
	double a, b, c, d, e, f;

	a = 1.2;
	b = 0.1;
	c = 1.0;
	d = 0.1;
	e = 0.1;
	f = 0.1;

	double prev_x = 10;
	double prev_y = 20;
	
	double x, y;
	
	for (int t = 0; t < time_end; t++) {
		cout << prev_x << " " << prev_y << endl;
		x = prev_x + time_bunkai * (a - e * prev_x - b * prev_y);
		y = prev_y + time_bunkai * (- c + d * prev_x - f * prev_y);

		prev_x = x;
		prev_y = y;
		if (prev_x < 0) prev_x = 0;
		if (prev_y < 0) prev_y = 0;
	}

	return 0;
}
