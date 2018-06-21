#include <pidController.h>

// Calcula el proximo valor de la salida del controlador
void calculate_u(struct pid_param *params, struct pid_calc *calc)
{
	// Calculo de valores actuales
	calc->proportional = params->Kp * (params->beta * calc->setpoint - calc->procout);
	calc->der_term = (params->alpha * params->Td) / (params->alpha * params->Td + params->Ts);
	calc->derivative = calc->der_term * calc->past_derivative - calc->der_term * (calc->procout - calc->past_procout);
	calc->ctrlsignal_u = calc->proportional + calc->integral + calc->derivative; // salida del controlador
	// Calculo de valores anteriores
	calc->past_derivative = calc->derivative;
	calc->past_setpoint = calc->setpoint;
	calc->past_procout = calc->procout;
	// Calculo de valores siguientes
	calc->int_term = params->Ts * params->Kp / params->Ti;
	calc->next_integral = calc->integral + calc->int_term * (calc->procout - calc->setpoint);
};
