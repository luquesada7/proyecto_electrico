#ifndef pidController_H
#define pidController_H

class pidController {
public:
	// Constructor
	pidController(double input_Ts, double input_Kp, double input_Ti, double input_Td, double input_beta, double input_alpha) {
	};
	double getKp() { return Kp; };
	virtual calculate_u(void);
	//float get_u(void) { return ctrlsignal_u; };

private:
	// Parametros del PID
	double Ts;
	double Kp;
	double Ti;
	double Td;
	double beta;
	double alpha;
	// Calculo
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
}

#endif // pidController_H
