#ifndef __EULER_SOLVER_H
#define __EULER_SOLVER_H

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


double m(float t) {
	double Dg = 40.0;
	double Ag = 0.8;
	double tmax_I = 55.0;
	double tmax_G = 40.0;
	double tiempo_comida = 0.0;
	double Vg = 13.79;
	double m;

	if (t < tiempo_comida) {
		m = 0;
	}
	else {
		t = t - tiempo_comida;
		m = 100 * Dg*Ag*t*exp(-t / tmax_I) / (Vg*pow(tmax_G, 2));
		//printf("%lf\n", m);
	}
	return m;

};

void eulerSolver(struct parameters *param, struct states *currentStates) {
	double I_prox, X_prox, G_prox, t_prox;

	I_prox = currentStates->I + param->h * ((-param->n * currentStates->I) + (param->tau * param->u));
	X_prox = currentStates->X + param->h * ((-param->p2 * currentStates->X) + (param->p3 * currentStates->I));
	G_prox = currentStates->G + param->h * (-(param->p1 + currentStates->X)*(currentStates->G) + param->p1 * param->Gb + m(currentStates->t));
	t_prox = currentStates->t + param->h;

	currentStates->t = t_prox;
	currentStates->I = I_prox;
	currentStates->G = G_prox;
	currentStates->X = X_prox;
};

void rungeKutta4Solver(struct parameters *param, struct states *currentStates) {

	double a1, b1, c1, t_1, I_1, X_1, G_1, a2, b2, c2, t_2, I_2, X_2, G_2, a3, b3, c3, t_3, I_3, X_3, G_3, a4, b4, c4, t_prox, I_prox, X_prox, G_prox;

	a1 = (-param->n * currentStates->I) + (param->tau * param->u);
	b1 = (-param->p2 * currentStates->X) + (param->p3 * currentStates->I);
	c1 = -(param->p1 + currentStates->X)*currentStates->G + param->p1 * param->Gb + m(currentStates->t);

	t_1 = currentStates->t + (param->h / 2.0);
	I_1 = currentStates->I + (param->h / 2.0) * a1;
	X_1 = currentStates->X + (param->h / 2.0) * b1;
	G_1 = currentStates->G + (param->h / 2.0) * c1;
	a2 = (-param->n * I_1) + (param->tau * param->u);
	b2 = (-param->p2 * X_1) + (param->p3 * I_1);
	c2 = -(param->p1 + X_1)*currentStates->G + param->p1 * param->Gb + m(t_1);

	t_2 = currentStates->t + (param->h / 2.0);
	I_2 = currentStates->I + (param->h / 2.0) * a2;
	X_2 = currentStates->X + (param->h / 2.0) * b2;
	G_2 = currentStates->G + (param->h / 2.0) * c2;
	a3 = (-param->n * I_2) + (param->tau * param->u);
	b3 = (-param->p2 * X_2) + (param->p3 * I_2);
	c3 = -(param->p1 + X_2)*currentStates->G + param->p1 * param->Gb + m(t_2);

	t_3 = currentStates->t + param->h;
	I_3 = currentStates->I + param->h * a3;
	X_3 = currentStates->X + param->h * b3;
	G_3 = currentStates->G + param->h * c3;
	a4 = (-param->n * I_3) + (param->tau * param->u);
	b4 = (-param->p2 * X_3) + (param->p3 * I_3);
	c4 = -(param->p1 + X_3)*currentStates->G + param->p1 * param->Gb + m(t_3);

	t_prox = currentStates->t + param->h;
	I_prox = currentStates->I + (param->h / 6.0) * (a1 + 2.0 * a2 + 2.0 * a3 + a4);
	X_prox = currentStates->X + (param->h / 6.0) * (b1 + 2.0 * b2 + 2.0 * b3 + b4);
	G_prox = currentStates->G + (param->h / 6.0) * (c1 + 2.0 * c2 + 2.0 * c3 + c4);

	currentStates->t = t_prox;
	currentStates->I = I_prox;
	currentStates->G = G_prox;
	currentStates->X = X_prox;

};
#endif
