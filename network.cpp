#include <iostream>
#include <math.h>

using namespace std;
int i, j;
const int max_x = 1000, max_i = 1000, max_j = 1000; //max_xがノード数
int J[max_i][max_j]; //iにjが作用する
double x[max_x];
int number;
double theta;
double ref;
double kp;
double next_x[max_x];
double t = 0;
int time_end;

int main(void)
{
	/*
	cout << "Input the number of nodes. -> ";
	cin >> number;
	while(number > max_x) {
		cout << "Invalid number. Try again." << endl;
		cin >> number;
	}
	for (i = 0; i < number; i++) {
		for (j = 0; j < number; j++) {
			cout << "Input J[" << i << "][" << j << "] -> ";
			cin >> J[i][j];
		}
	}
	for (i = 0; i < number; i++) {
		cout << "Input the initial x[" << i << "] -> ";
		cin >> x[i];
	}

	cout << "Input the theta -> ";
	cin >> theta;

	cout << "Input the kp -> ";
	cin >> kp;
	while(kp <= 0) {
		cout << "Invalid. Try again ->";
		cin >> kp;
	}
	
	cout << "Input the reflection -> ";
	cin >> ref;

	cout << "Input the time to end -> ";
	cin >> time_end;
	
	//式は (next_x[i] - x[i]) / 0.01 = 1 / kp *  Σ (J[i][j] * x[j] - theta) - ref * x[i];	
	*/

	number = 5;
	J[0][0] = 1;
	J[0][1] = -1;
	J[0][2] = 1;
	J[0][3] = 0;
	J[0][4] = 0;
	J[1][0] = -1;
	J[1][1] = 1;
	J[1][2] = 1;
	J[1][3] = 0;
	J[1][4] = 0;
	J[2][0] = 0;
	J[2][1] = 0;
	J[2][2] = 1;
	J[2][3] = -3;
	J[2][4] = 4;
	J[3][0] = 1;
	J[3][1] = 1;
	J[3][2] = 1;
	J[3][3] = 1;
	J[3][4] = 4;
	J[4][0] = 0;
	J[4][1] = 0;
	J[4][2] = 0;
	J[4][3] = 0;
	J[4][4] = -1;
	x[0] = 0;
	x[1] = 0;
	x[2] = 1;
	x[3] = 0;
	x[4] = 0;
	theta = 0;
	kp = 10;
	ref = 0.1;
	time_end = 10000;

	cout << 0 << " ";
	for (i = 0; i < number; i++) {
		cout << x[i] << " ";
	}
	cout << endl;
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

	return 0;
}

