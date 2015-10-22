#include <iostream>

using namespace std;

double next_x; 
double x;
double t;
double k;
double r;
int time_end;
double time_bunkai;

int main(void)
{
	cout << "Equation will be dx = r * x * (1 - x / k)" << endl;
	cout << "Input k ->" ;
	cin >> k;
	while (k == 0) {
		cout << "Invalid. Try again ->";
		cin >> k;
	}
	cout << "Input r ->" ;
	cin >> r;
	cout << "Input the smallest time [s]->";
	cin >> time_bunkai;
	while (time_bunkai <= 0) {
		cout << "Invalid. Try again ->";
		cin >> time_bunkai;
	}
	cout << "Input the initail x ->";
	cin >> x;
	cout << "Input the time to end [time * time_bunkai] ->";
	cin >> time_end;
	t = 0;
	cout << t << " " << x << endl;
	
	while (t < time_end) {
		t++;
		next_x = x + time_bunkai * r * x * (1 - x / k);
		x = next_x;
		cout << t * time_bunkai << " " << x << endl;
	}

	return 0;
}

