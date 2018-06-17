#include <stdio.h>
#include <math.h>
#include <modelos.h>
#include <lqrController.h>
#include <pidController.h>

int pid(void)
{
	struct pid_param params = { 0 };
	struct pid_calc calc = { 0 };

	params.Ts = 1;
	params.Kp = -1.228;
	params.Ti = 81.26;
	params.Td = 19.5;
	params.beta = 1;
	params.alpha = 0.01;

	calculate_u(&params, &calc);
	return 0;
};

int lqr(void)
{
	// Inicializacion de matrices del controlador
	struct matG G;
	struct matH H;
	struct matC C;
	struct matKe Ke;
	struct matK1 K1;
	struct matK2 K2;
	struct lqr_states currentStates;

	G.G11 = 0.9523;
	G.G12 = 0.0302;
	G.G13 = 0.0008;
	G.G21 = -0.1906;
	G.G22 = 0.8342;
	G.G23 = 0.0658;
	G.G31 = -0.2211;
	G.G32 = 0.5331;
	G.G33 = 0.3039;
	H.H11 = 0.0001;
	H.H21 = -0.0130;
	H.H31 = -0.1167;
	C.C11 = 9.1569;
	C.C12 = 0.1399;
	C.C13 = 0.0044;

	Ke.Ke_11 = 0.0398;
	Ke.Ke_21 = -0.0135;
	Ke.Ke_31 = -0.0076;
	K1.K1_11 = -0.0182;
	K2.K2_11 = -1.9176;
	K2.K2_11 = -0.2889;
	K2.K2_13 = -0.0314;

	// Inicializacion de los estados iniciales
	currentStates.x1 = 0.0;
	currentStates.x2 = 0.0;
	currentStates.x3 = 0.0;

	// Inicializacion de otros parametros
	double r_act = 120.0;
	double y_act = 270.0;
	double I_past = 0.0;
	double ctrlsig_u = 0.0;

	ctrlsig_u = lqrController(&G, &H, &C, &Ke, &K1, &K2, &currentStates, r_act, y_act, &I_past);

	printf("Ctrl signal: %lf\n", ctrlsig_u);

	return 0;
};

int modelos(void)
{
	struct states currentStates = {};
	struct parameters param = {};

	// Condiciones iniciales
	currentStates.I = 0.0;
	currentStates.X = 0.0;
	currentStates.G = 120.0;
	currentStates.t = 0.0;

	// Paciente
	param.p1 = 0.0337;
	param.p2 = 0.0209;
	param.p3 = 7.5 * pow(10, -6);
	param.tau = 1 / 12;
	param.n = 0.214;
	param.Gb = 120.0;
	param.u = 48.0;
	param.h = 1.0;

	int x = 0;

	FILE *f = fopen("file2.txt", "w");
	if (f == NULL) {
		return -1;
	}

	while (x < 3000) {
		fprintf(f, "%lf, %lf\n", currentStates.t, currentStates.G);
		//eulerSolver(&param, &currentStates);
		rungeKutta4Solver(&param, &currentStates);
		x++;
	}

	fclose(f);
	return 0;
};

int main() {
	if (lqr() || pid() || modelos()) {
		return -1;
	}
	return 0;
}
