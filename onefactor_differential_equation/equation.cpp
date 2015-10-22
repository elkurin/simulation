#include <iostream>
#include <math.h>
#include "function.h"
#include "variable.h"

using namespace std;

typedef struct
{
	double r;
	double k;
} coefficient;

coefficient factor;

void need(void)
{
	cout << "Equation is dx = rx*(1+kx)" << endl;
	cout << "Input r -> ";
	cin >> factor.r;
	cout << "Input k -> ";
	cin >> factor.k;
	while (factor.k == 0) {
		cout << "Invalid. Try again -> ";
		cin >> factor.k;
	}
}

double bibun_equation(double t, double x)
{
	return 3 * x;
}

