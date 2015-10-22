#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(void)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 1000; i++) {
		int o = rand() % 10000;
		double p = o * 0.0001;
		cout << p << endl;
	}
	return 0;
}
