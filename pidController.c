// v.1
// Date: 02/06/2018
// Universidad de Costa Rica, Facultad de Ingeniería Eléctrica 
// Luisa Fernanda Quesada, luisa.quesadacamacho@ucr.ac.cr
// PID_Control.c
#include <stdio.h>
#include <math.h>
#include <C:\Users\lfquesad\Documents\Personal\Proyecto Eléctrico\C_code\pidController.h>

// Pone los parametros del controlador
pidController::pidController(double input_Ts, double input_Kp, double input_Ti, double input_Td, double input_beta, double input_alpha) {
	Ts = input_Ts;
	Kp = input_Kp;
	Ti = input_Ti;
	Td = input_Td;
	beta = input_beta;
	alpha = input_alpha;
};

// Calcula el proximo valor de la salida del controlador
virtual pidController::calculate_u(void) {
	// Calculo de valores actuales
	proportional = Kp * (beta*setpoint - procout);
	der_term = (alpha*Ts) / (alpha*Td + Ts);
	derivative = der_term * past_derivative - der_term * (procout - past_procout);
	ctrlsignal_u = proportional + integral + derivative; // salida del controlador
	// Calculo de valores anteriores
	past_derivative = derivative;
	past_setpoint = setpoint;
	past_procout = procout;
	// Calculo de valores siguientes
	int_term = Ts * Kp / Ti;
	next_integral = integral + int_term * (procout - setpoint);
};

int main() {
	pidController pid(1, -1.228, 81.26, 19.5, 1, 0.01);
	getKp();
};
