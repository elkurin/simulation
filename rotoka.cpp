#include <iostream>

using namespace std;
int time_end = 30;
double time_bunkai = 0.01;

double bunkai = 0.1;

int main(void)
{
	double a = 0.1;
	double b = 0.1;
	double c = 0.1;
	double d = 0.1;

	double x = 0;
	double y = 0;

	double prev_x = 50;
	double prev_y = 10;

	double init_x;
	double init_y;

	int match = 0;

	a = bunkai;

	while(1) {//a
		b = bunkai;
		while(1) {//b
			c = bunkai;
			while(1) {//c
				d = bunkai;
				while(1) {//d
					init_x = 10;
					while(1) {//init_x
						init_y = 10;
						while(1) {//init_y
							prev_x = init_x;
							prev_y = init_y;

							for (int t = 0; t < time_end; t++) {
								//cout << t << " " << prev_x << " " << prev_y << endl;
								x = prev_x + time_bunkai * prev_x * (a - b * prev_y);
								y = prev_y + time_bunkai * prev_y * (- c + d * prev_x);
								prev_x = x;
								prev_y = y;
								if (prev_x < 0) prev_x = 0;
								if (prev_y < 0) prev_y = 0;
							}
							if (x > 1 && y > 1) {
								for (int t = 0; t < time_end; t++) {
									//cout << t << " " << prev_x << " " << prev_y << endl;
									x = prev_x + time_bunkai * prev_x * (a - b * prev_y);
									y = prev_y + time_bunkai * prev_y * (- c + d * prev_x);
									prev_x = x;
									prev_y = y;
									if (prev_x < 0) prev_x = 0;
									if (prev_y < 0) prev_y = 0;
								}
								if (x > 1 && y > 1) {
									double min_x = 10000;
									double min_y = 10000;
									for (int t = 0; t < time_end * 20; t++) {
										//cout << t << " " << prev_x << " " << prev_y << endl;
										x = prev_x + time_bunkai * prev_x * (a - b * prev_y);
										y = prev_y + time_bunkai * prev_y * (- c + d * prev_x);
										prev_x = x;
										prev_y = y;
										if (prev_x < 0) prev_x = 0;
										if (prev_y < 0) prev_y = 0;
										if (min_x > x) min_x = x;
										if (min_x > y) min_y = y;
									}
									if (x > 1 && y > 1) {
										double minx = 10000;
										double miny = 10000;
										for (int t = 0; t < time_end * 20; t++) {
											//cout << t << " " << prev_x << " " << prev_y << endl;
											x = prev_x + time_bunkai * prev_x * (a - b * prev_y);
											y = prev_y + time_bunkai * prev_y * (- c + d * prev_x);
											prev_x = x;
											prev_y = y;
											if (prev_x < 0) prev_x = 0;
											if (prev_y < 0) prev_y = 0;
											if (minx > x) minx = x;
											if (miny > y) miny = y;
										}
										if (x > init_x / 10 && y > init_y / 10 && min_x < minx && min_y < miny) {
											cout << a << " " << b << " " << c << " " << d << " " << init_x << " " << init_y << endl;
											match = 1;
											break;
										}
									}
								}
							}
							if (init_y > 200) break;
							init_y += 10;
						}
						if (init_x > 200) break;
						init_x += 10;
					}
					if (d > 3) {
						break;
					}
					d += bunkai;
				}
				if (c > 3) break;
				c += bunkai;
			}
			if (b > 3) break;
			b += bunkai;
		}
		if (a > 3) break;
		a += bunkai;
	}

	if (match == 0) {
		cout << "no match" << endl;
	}

	return 0;
}
		
