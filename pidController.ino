#include <Arduino_FreeRTOS.h>
#include <math.h>
#include <stdio.h>
#include <pidController.h>

// Calcula el proximo valor de la salida del controlador
double pidController(struct pid_param *params, struct pid_calc *calc)
{
	double ctrlsig_u;

	// Calculo de valores actuales
	calc->proportional = params->Kp * (params->beta * calc->setpoint - calc->procout);
	calc->der_term = (params->alpha * params->Td) / (params->alpha * params->Td + params->Ts);
	calc->derivative = calc->der_term * calc->past_derivative - calc->der_term * (calc->procout - calc->past_procout);
	calc->ctrlsignal_u = 48.2328 + calc->proportional + calc->integral + calc->derivative; // salida del controlador

																						   // Calculo de valores anteriores
	calc->past_derivative = calc->derivative;
	calc->past_setpoint = calc->setpoint;
	calc->past_procout = calc->procout;
	calc->integral = calc->next_integral;

	// Calculo de valores siguientes
	calc->int_term = params->Ts * params->Kp / params->Ti;
	calc->next_integral = calc->integral + calc->int_term * (calc->setpoint - calc->procout);

	ctrlsig_u = calc->ctrlsignal_u;
	return ctrlsig_u;
};

void setup() {

	Wire.begin();        // join i2c bus (address optional for master)
	Serial.begin(9600);

	struct pid_param params = { 0 };
	struct pid_calc calc = { 0 };

	params.Ts = 1;
	params.Kp = -1.228;
	params.Ti = 81.26;
	params.Td = 19.5;
	params.beta = 1;
	params.alpha = 0.01;

	calc.past_procout = 120.0;
	calc.setpoint = calc.past_setpoint = 120.0;

	double ctrlsig_u;

	// Tareas de FreeRTOS
	xTaskCreate(
		Taski2c
		, (const portCHAR *)"i2c"   // A name just for humans
		, 128  // Stack size
		, NULL
		, 1  // priority
		, NULL);

	xTaskCreate(
		TaskPidController
		, (const portCHAR *) "pidController"
		, 128 // This stack size can be checked & adjusted by reading Highwater
		, NULL
		, 2  // priority
		, NULL);
}

void loop()
{
	// Empty. Things are done in Tasks.
}

void TaskController(void *pvParameters)  // This is a task.
{
	(void)pvParameters;

	for (;;)
	{
		ctrlsig_u = pidController(&params, &calc);
		Serial.println(ctrlsig_u);
		vTaskDelay(1000 / 15 * portTICK_PERIOD_MS);  // one tick delay (15ms) in between writes for stability
	}
}
