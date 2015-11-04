#pragma once

const int M = 1;
const int N = 20;

typedef struct {
//	double possibility; //ここのネットワークがつながる確率
	int connected;
//	double coef;
	int catalyst;
} Network;

typedef struct {
	Network network[N][N];
	double node[N];//concentration
	double prev_node[N];
	double size;
	double init_size;
} Cell;
