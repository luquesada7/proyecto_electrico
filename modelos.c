#include <stdio.h>  
#include <math.h>
#include <C:\Users\lfquesad\Documents\Personal\Proyecto Eléctrico\C_code\euler_solver.h>

int main() {

	struct states currentStates;
	struct parameters param;

	//Condiciones iniciales
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
	if (f == NULL) return -1;

	while (x < 3000) {
		fprintf(f, "%lf, %lf\n", currentStates.t, currentStates.G);
		//eulerSolver(&param, &currentStates);
		rungeKutta4Solver(&param, &currentStates);
		x++;
	};

	fclose(f);
	return 0;
};
