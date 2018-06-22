#include <Arduino_FreeRTOS.h>
#include <math.h>
#include <stdio.h>
#include <lqrController.h>

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
	double *I_past)
{
	double I_act;
	double ctrlsig_u;
	double x1_next;
	double x2_next;
	double x3_next;

	// Valores actuales
	I_act = *I_past + r_act - y_act;
	ctrlsig_u = K1->K1_11 * I_act - (K2->K2_11 * x->x1 + K2->K2_12 * x->x2 + K2->K2_13 * x->x3) + 48.2328;

	// Guardar valor actual
	*I_past = I_act;

	// Valores siguientes
	x1_next = G->G11 * x->x1 + G->G12 * x->x2 + G->G13 * x->x3 + H->H11 * ctrlsig_u + Ke->Ke_11 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);
	x2_next = G->G21 * x->x1 + G->G22 * x->x2 + G->G23 * x->x3 + H->H21 * ctrlsig_u + Ke->Ke_21 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);
	x3_next = G->G31 * x->x1 + G->G32 * x->x2 + G->G33 * x->x3 + H->H31 * ctrlsig_u + Ke->Ke_31 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);

	return ctrlsig_u;
}

void setup() {

	Wire.begin();        // join i2c bus (address optional for master)
	Serial.begin(9600);

	//Inicializacion de matrices del controlador
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
	G.G32 = -0.5331;
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

	//Inicializacion de otros parametros 
	double r_act = 120.0;
	//double y_act[2001] = { 120 , 120 , 120.01 , 120.01 , 120.02 , 120.02 , 120.02 , 120.03 , 120.03 , 120.03 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 120.09 , 120.4 , 120.89 , 121.56 , 122.38 , 123.35 , 124.44 , 125.66 , 126.99 , 128.42 , 129.93 , 131.53 , 133.19 , 134.92 , 136.7 , 138.53 , 140.4 , 142.3 , 144.22 , 146.17 , 148.14 , 150.11 , 152.09 , 154.07 , 156.05 , 158.02 , 159.98 , 161.92 , 163.84 , 165.75 , 167.62 , 169.48 , 171.3 , 173.09 , 174.84 , 176.56 , 178.25 , 179.89 , 181.49 , 183.05 , 184.56 , 186.03 , 187.46 , 188.83 , 190.16 , 191.44 , 192.67 , 193.85 , 194.98 , 196.06 , 197.09 , 198.07 , 199 , 199.87 , 200.69 , 201.47 , 202.19 , 202.86 , 203.47 , 204.04 , 204.56 , 205.02 , 205.44 , 205.81 , 206.13 , 206.4 , 206.63 , 206.8 , 206.93 , 207.02 , 207.06 , 207.06 , 207.01 , 206.92 , 206.79 , 206.62 , 206.41 , 206.16 , 205.87 , 205.55 , 205.19 , 204.79 , 204.36 , 203.89 , 203.4 , 202.87 , 202.31 , 201.72 , 201.1 , 200.46 , 199.79 , 199.09 , 198.37 , 197.62 , 196.85 , 196.06 , 195.25 , 194.42 , 193.57 , 192.71 , 191.82 , 190.92 , 190.01 , 189.08 , 188.13 , 187.18 , 186.21 , 185.23 , 184.24 , 183.25 , 182.24 , 181.23 , 180.2 , 179.18 , 178.15 , 177.11 , 176.07 , 175.02 , 173.97 , 172.92 , 171.87 , 170.82 , 169.77 , 168.72 , 167.66 , 166.61 , 165.56 , 164.52 , 163.47 , 162.43 , 161.39 , 160.36 , 159.33 , 158.3 , 157.29 , 156.27 , 155.26 , 154.26 , 153.26 , 152.27 , 151.29 , 150.31 , 149.35 , 148.39 , 147.43 , 146.49 , 145.55 , 144.62 , 143.7 , 142.79 , 141.89 , 140.99 , 140.11 , 139.23 , 138.37 , 137.51 , 136.66 , 135.82 , 134.99 , 134.17 , 133.36 , 132.56 , 131.77 , 130.99 , 130.22 , 129.46 , 128.7 , 127.96 , 127.23 , 126.51 , 125.79 , 125.09 , 124.39 , 123.71 , 123.03 , 122.37 , 121.71 , 121.06 , 120.42 , 119.79 , 119.17 , 118.56 , 117.96 , 117.37 , 116.78 , 116.21 , 115.64 , 115.09 , 114.54 , 114 , 113.46 , 112.94 , 112.42 , 111.92 , 111.42 , 110.93 , 110.44 , 109.97 , 109.5 , 109.04 , 108.59 , 108.14 , 107.7 , 107.27 , 106.85 , 106.44 , 106.03 , 105.63 , 105.23 , 104.84 , 104.46 , 104.09 , 103.72 , 103.36 , 103.01 , 102.66 , 102.32 , 101.98 , 101.65 , 101.33 , 101.01 , 100.7 , 100.39 , 100.09 , 99.8 , 99.511 , 99.228 , 98.95 , 98.678 , 98.411 , 98.15 , 97.893 , 97.642 , 97.395 , 97.154 , 96.917 , 96.686 , 96.459 , 96.237 , 96.019 , 95.807 , 95.598 , 95.394 , 95.195 , 95 , 94.809 , 94.623 , 94.44 , 94.262 , 94.088 , 93.918 , 93.752 , 93.59 , 93.431 , 93.277 , 93.126 , 92.979 , 92.836 , 92.696 , 92.56 , 92.427 , 92.298 , 92.172 , 92.05 , 91.931 , 91.815 , 91.702 , 91.593 , 91.487 , 91.384 , 91.284 , 91.187 , 91.094 , 91.003 , 90.915 , 90.83 , 90.748 , 90.668 , 90.592 , 90.518 , 90.447 , 90.378 , 90.312 , 90.249 , 90.189 , 90.131 , 90.075 , 90.022 , 89.971 , 89.923 , 89.877 , 89.834 , 89.792 , 89.754 , 89.717 , 89.682 , 89.65 , 89.62 , 89.592 , 89.567 , 89.543 , 89.521 , 89.502 , 89.484 , 89.469 , 89.455 , 89.444 , 89.434 , 89.426 , 89.42 , 89.416 , 89.414 , 89.414 , 89.415 , 89.418 , 89.423 , 89.429 , 89.438 , 89.448 , 89.459 , 89.472 , 89.487 , 89.503 , 89.521 , 89.541 , 89.562 , 89.584 , 89.608 , 89.634 , 89.661 , 89.689 , 89.719 , 89.75 , 89.783 , 89.817 , 89.852 , 89.889 , 89.927 , 89.966 , 90.006 , 90.048 , 90.091 , 90.135 , 90.181 , 90.227 , 90.275 , 90.324 , 90.375 , 90.426 , 90.478 , 90.532 , 90.587 , 90.642 , 90.699 , 90.757 , 90.816 , 90.876 , 90.937 , 90.999 , 91.061 , 91.125 , 91.19 , 91.256 , 91.323 , 91.39 , 91.459 , 91.528 , 91.599 , 91.67 , 91.742 , 91.815 , 91.889 , 91.963 , 92.039 , 92.115 , 92.192 , 92.27 , 92.348 , 92.428 , 92.508 , 92.588 , 92.67 , 92.752 , 92.835 , 92.919 , 93.003 , 93.088 , 93.173 , 93.26 , 93.347 , 93.434 , 93.522 , 93.611 , 93.7 , 93.79 , 93.881 , 93.972 , 94.064 , 94.156 , 94.249 , 94.342 , 94.436 , 94.53 , 94.625 , 94.72 , 94.816 , 94.912 , 95.009 , 95.107 , 95.204 , 95.302 , 95.401 , 95.5 , 95.599 , 95.699 , 95.799 , 95.9 , 96.001 , 96.103 , 96.204 , 96.306 , 96.409 , 96.512 , 96.615 , 96.718 , 96.822 , 96.926 , 97.031 , 97.135 , 97.24 , 97.346 , 97.451 , 97.557 , 97.663 , 97.77 , 97.876 , 97.983 , 98.09 , 98.197 , 98.305 , 98.413 , 98.521 , 98.629 , 98.737 , 98.846 , 98.954 , 99.063 , 99.172 , 99.281 , 99.39 , 99.5 , 99.609 , 99.719 , 99.829 , 99.939 , 100.05 , 100.16 , 100.27 , 100.38 , 100.49 , 100.6 , 100.71 , 100.82 , 100.93 , 101.04 , 101.15 , 101.26 , 101.37 , 101.49 , 101.6 , 101.71 , 101.82 , 101.93 , 102.04 , 102.15 , 102.26 , 102.37 , 102.48 , 102.59 , 102.7 , 102.81 , 102.93 , 103.04 , 103.15 , 103.26 , 103.37 , 103.48 , 103.59 , 103.7 , 103.81 , 103.92 , 104.03 , 104.14 , 104.25 , 104.36 , 104.46 , 104.57 , 104.68 , 104.79 , 104.9 , 105.01 , 105.12 , 105.23 , 105.33 , 105.44 , 105.55 , 105.66 , 105.76 , 105.87 , 105.98 , 106.08 , 106.19 , 106.3 , 106.4 , 106.51 , 106.61 , 106.72 , 106.82 , 106.93 , 107.03 , 107.14 , 107.24 , 107.35 , 107.45 , 107.55 , 107.66 , 107.76 , 107.86 , 107.96 , 108.07 , 108.17 , 108.27 , 108.37 , 108.47 , 108.57 , 108.67 , 108.77 , 108.87 , 108.97 , 109.07 , 109.17 , 109.27 , 109.36 , 109.46 , 109.56 , 109.66 , 109.75 , 109.85 , 109.94 , 110.04 , 110.14 , 110.23 , 110.32 , 110.42 , 110.51 , 110.61 , 110.7 , 110.79 , 110.88 , 110.97 , 111.07 , 111.16 , 111.25 , 111.34 , 111.43 , 111.52 , 111.61 , 111.69 , 111.78 , 111.87 , 111.96 , 112.04 , 112.13 , 112.22 , 112.3 , 112.39 , 112.47 , 112.56 , 112.64 , 112.72 , 112.81 , 112.89 , 112.97 , 113.05 , 113.13 , 113.21 , 113.3 , 113.38 , 113.45 , 113.53 , 113.61 , 113.69 , 113.77 , 113.85 , 113.92 , 114 , 114.07 , 114.15 , 114.22 , 114.3 , 114.37 , 114.45 , 114.52 , 114.59 , 114.66 , 114.74 , 114.81 , 114.88 , 114.95 , 115.02 , 115.09 , 115.16 , 115.22 , 115.29 , 115.36 , 115.43 , 115.49 , 115.56 , 115.62 , 115.69 , 115.75 , 115.82 , 115.88 , 115.94 , 116.01 , 116.07 , 116.13 , 116.19 , 116.25 , 116.31 , 116.37 , 116.43 , 116.49 , 116.55 , 116.61 , 116.66 , 116.72 , 116.78 , 116.83 , 116.89 , 116.94 , 117 , 117.05 , 117.11 , 117.16 , 117.21 , 117.26 , 117.32 , 117.37 , 117.42 , 117.47 , 117.52 , 117.57 , 117.62 , 117.67 , 117.71 , 117.76 , 117.81 , 117.86 , 117.9 , 117.95 , 117.99 , 118.04 , 118.08 , 118.13 , 118.17 , 118.21 , 118.25 , 118.3 , 118.34 , 118.38 , 118.42 , 118.46 , 118.5 , 118.54 , 118.58 , 118.62 , 118.66 , 118.7 , 118.73 , 118.77 , 118.81 , 118.84 , 118.88 , 118.91 , 118.95 , 118.98 , 119.02 , 119.05 , 119.08 , 119.12 , 119.15 , 119.18 , 119.21 , 119.24 , 119.28 , 119.31 , 119.34 , 119.37 , 119.4 , 119.42 , 119.45 , 119.48 , 119.51 , 119.54 , 119.56 , 119.59 , 119.62 , 119.64 , 119.67 , 119.69 , 119.72 , 119.74 , 119.77 , 119.79 , 119.82 , 119.84 , 119.86 , 119.88 , 119.91 , 119.93 , 119.95 , 119.97 , 119.99 , 120.01 , 120.03 , 120.05 , 120.07 , 120.09 , 120.11 , 120.13 , 120.15 , 120.16 , 120.18 , 120.2 , 120.22 , 120.23 , 120.25 , 120.26 , 120.28 , 120.3 , 120.31 , 120.33 , 120.34 , 120.36 , 120.37 , 120.38 , 120.4 , 120.41 , 120.42 , 120.44 , 120.45 , 120.46 , 120.47 , 120.48 , 120.5 , 120.51 , 120.52 , 120.53 , 120.54 , 120.55 , 120.56 , 120.57 , 120.58 , 120.59 , 120.6 , 120.61 , 120.62 , 120.62 , 120.63 , 120.64 , 120.65 , 120.66 , 120.66 , 120.67 , 120.68 , 120.68 , 120.69 , 120.7 , 120.7 , 120.71 , 120.71 , 120.72 , 120.73 , 120.73 , 120.74 , 120.74 , 120.75 , 120.75 , 120.75 , 120.76 , 120.76 , 120.77 , 120.77 , 120.77 , 120.78 , 120.78 , 120.78 , 120.79 , 120.79 , 120.79 , 120.79 , 120.8 , 120.8 , 120.8 , 120.8 , 120.8 , 120.8 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.81 , 120.8 , 120.8 , 120.8 , 120.8 , 120.8 , 120.8 , 120.8 , 120.79 , 120.79 , 120.79 , 120.79 , 120.79 , 120.78 , 120.78 , 120.78 , 120.78 , 120.77 , 120.77 , 120.77 , 120.77 , 120.76 , 120.76 , 120.76 , 120.75 , 120.75 , 120.75 , 120.75 , 120.74 , 120.74 , 120.74 , 120.73 , 120.73 , 120.73 , 120.72 , 120.72 , 120.72 , 120.71 , 120.71 , 120.7 , 120.7 , 120.7 , 120.69 , 120.69 , 120.69 , 120.68 , 120.68 , 120.67 , 120.67 , 120.67 , 120.66 , 120.66 , 120.65 , 120.65 , 120.65 , 120.64 , 120.64 , 120.63 , 120.63 , 120.63 , 120.62 , 120.62 , 120.61 , 120.61 , 120.6 , 120.6 , 120.6 , 120.59 , 120.59 , 120.58 , 120.58 , 120.57 , 120.57 , 120.57 , 120.56 , 120.56 , 120.55 , 120.55 , 120.54 , 120.54 , 120.54 , 120.53 , 120.53 , 120.52 , 120.52 , 120.51 , 120.51 , 120.5 , 120.5 , 120.5 , 120.49 , 120.49 , 120.48 , 120.48 , 120.47 , 120.47 , 120.47 , 120.46 , 120.46 , 120.45 , 120.45 , 120.44 , 120.44 , 120.44 , 120.43 , 120.43 , 120.42 , 120.42 , 120.41 , 120.41 , 120.41 , 120.4 , 120.4 , 120.39 , 120.39 , 120.38 , 120.38 , 120.38 , 120.37 , 120.37 , 120.36 , 120.36 , 120.36 , 120.35 , 120.35 , 120.34 , 120.34 , 120.34 , 120.33 , 120.33 , 120.32 , 120.32 , 120.32 , 120.31 , 120.31 , 120.31 , 120.3 , 120.3 , 120.29 , 120.29 , 120.29 , 120.28 , 120.28 , 120.28 , 120.27 , 120.27 , 120.27 , 120.26 , 120.26 , 120.25 , 120.25 , 120.25 , 120.24 , 120.24 , 120.24 , 120.23 , 120.23 , 120.23 , 120.22 , 120.22 , 120.22 , 120.21 , 120.21 , 120.21 , 120.21 , 120.2 , 120.2 , 120.2 , 120.19 , 120.19 , 120.19 , 120.18 , 120.18 , 120.18 , 120.17 , 120.17 , 120.17 , 120.17 , 120.16 , 120.16 , 120.16 , 120.16 , 120.15 , 120.15 , 120.15 , 120.14 , 120.14 , 120.14 , 120.14 , 120.13 , 120.13 , 120.13 , 120.13 , 120.12 , 120.12 , 120.12 , 120.12 , 120.11 , 120.11 , 120.11 , 120.11 , 120.11 , 120.1 , 120.1 , 120.1 , 120.1 , 120.09 , 120.09 , 120.09 , 120.09 , 120.09 , 120.08 , 120.08 , 120.08 , 120.08 , 120.08 , 120.07 , 120.07 , 120.07 , 120.07 , 120.07 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.06 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.05 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.04 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.03 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.02 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120.01 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.96 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.97 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.98 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 119.99 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120 };
	double I_past = 0.0;
	double ctrlsig_u = 48.2328;

	// Tareas de FreeRTOS
	xTaskCreate(
		Taski2c
		, (const portCHAR *)"i2c"   // A name just for humans
		, 128  // Stack size
		, NULL
		, 1  // priority
		, NULL);

	xTaskCreate(
		TaskLqrController
		, (const portCHAR *) "lqrController"
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
		ctrlsig_u = lqrController(&G, &H, &C, &Ke, &K1, &K2, &currentStates, r_act, glucose, &I_past);
		Serial.println(ctrlsig_u);
		vTaskDelay(1000 / 15 * portTICK_PERIOD_MS);  // one tick delay (15ms) in between writes for stability
	}
}
