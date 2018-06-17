#ifndef __MODELOS_H
#define __MODELOS_H

struct states {
	double G;
	double I;
	double X;
	double t;
};

struct parameters {
	double p1;
	double p2;
	double p3;
	double tau;
	double n;
	double Gb;
	double u;
	double h;
};

double m(float t);
void eulerSolver(struct parameters *param, struct states *currentStates);
void rungeKutta4Solver(struct parameters *param, struct states *currentStates);

#endif // __MODELOS_H
