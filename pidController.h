#ifndef __PIDCONTROLLER_H
#define __PIDCONTROLLER_H

#include <math.h>
#include <stdio.h>

struct pid_param {
	double Ts;
	double Kp;
	double Ti;
	double Td;
	double beta;
	double alpha;
};

struct pid_calc {
	double setpoint;
	double past_setpoint;
	double procout;
	double past_procout;
	double proportional;
	double derivative;
	double past_derivative;
	double integral;
	double next_integral;
	double ctrlsignal_u;
	double der_term;
	double int_term;
};

void calculate_u(struct pid_param *params, struct pid_calc *calc);

#endif // __PIDCONTROLLER_H
