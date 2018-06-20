#ifndef __LQRCONTROLLER_H
#define __LQRCONTROLLER_H

#include <stdio.h>

struct matG {
	double G11;
	double G12;
	double G13;
	double G21;
	double G22;
	double G23;
	double G31;
	double G32;
	double G33;
};

struct matH {
	double H11;
	double H21;
	double H31;
};

struct matC {
	double C11;
	double C12;
	double C13;
};

struct matKe {
	double Ke_11;
	double Ke_21;
	double Ke_31;
};

struct matK1 {
	double K1_11;
};

struct matK2 {
	double K2_11;
	double K2_12;
	double K2_13;
};

struct lqr_states {
	double x1;
	double x2;
	double x3;
};

double lqrController(
	struct matG *G,
	struct matH *H,
	struct matC *C,
	struct matKe *Ke,
	struct matK1 *K1,
	struct matK2 *K2,
	struct lqr_states *x,
	double r_act,
	double y_act,
	double *I_past);

#endif // __LQRCONTROLLER_H
