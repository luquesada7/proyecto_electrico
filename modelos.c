#include <stdio.h>  
#include <math.h>
#include <C:\Users\lfquesad\Documents\Personal\Proyecto Eléctrico\C_code\modelos.h>

double m(float t, struct food_parameters *food) {
	double m;
	if (t < food->tiempo_comida) {
		m = 0.0;
	}
	else {
		t = t - food->tiempo_comida;
		m = 100.0 * food->Dg*food->Ag*t*exp(-t / food->tmax_I) / (food->Vg*pow(food->tmax_G, 2.0));
	}
	return m;

};

void eulerSolver(struct parameters *param, struct states *currentStates, struct food_parameters *food) {
	double I_prox, X_prox, G_prox, t_prox;
	
	// Calculo de proximos valores
	I_prox = currentStates->I + param->h * ((-param->n * currentStates->I) + (param->tau * param->u));
	X_prox = currentStates->X + param->h * ((-param->p2 * currentStates->X) + (param->p3 * currentStates->I));
	G_prox = currentStates->G + param->h * (-(param->p1 + currentStates->X)*(currentStates->G) + param->p1 * param->Gb + m(currentStates->t, food));
	t_prox = currentStates->t + param->h;

	// Guardar en la estructura los valores calculados
	currentStates->t = t_prox;
	currentStates->I = I_prox;
	currentStates->G = G_prox;
	currentStates->X = X_prox;
};

void rungeKutta4Solver(struct parameters *param, struct states *currentStates, struct food_parameters *food) {
	
	// Inicializacion de parametros de Runge Kutta Orden 4
	double a1;
	double b1;
	double c1;
	double t_1;
	double I_1;
	double X_1;
	double G_1;
	double a2;
	double b2;
	double c2;
	double t_2;
	double I_2;
	double X_2;
	double G_2;
	double a3;
	double b3;
	double c3;
	double t_3;
	double I_3;
	double X_3;
	double G_3;
	double a4;
	double b4;
	double c4;
	double t_prox;
	double I_prox;
	double X_prox;
	double G_prox;

	// Ecuaciones de Runge Kutta Orden 4
	a1 = (-param->n * currentStates->I) + (param->tau * param->u);
	b1 = (-param->p2 * currentStates->X) + (param->p3 * currentStates->I);
	c1 = -(param->p1 + currentStates->X)*currentStates->G + param->p1 * param->Gb + m(currentStates->t, food);

	t_1 = currentStates->t + (param->h / 2.0);
	I_1 = currentStates->I + (param->h / 2.0) * a1;
	X_1 = currentStates->X + (param->h / 2.0) * b1;
	G_1 = currentStates->G + (param->h / 2.0) * c1;
	a2 = (-param->n * I_1) + (param->tau * param->u);
	b2 = (-param->p2 * X_1) + (param->p3 * I_1);
	c2 = -(param->p1 + X_1)*currentStates->G + param->p1 * param->Gb + m(t_1, food);

	t_2 = currentStates->t + (param->h / 2.0);
	I_2 = currentStates->I + (param->h / 2.0) * a2;
	X_2 = currentStates->X + (param->h / 2.0) * b2;
	G_2 = currentStates->G + (param->h / 2.0) * c2;
	a3 = (-param->n * I_2) + (param->tau * param->u);
	b3 = (-param->p2 * X_2) + (param->p3 * I_2);
	c3 = -(param->p1 + X_2)*currentStates->G + param->p1 * param->Gb + m(t_2, food);

	t_3 = currentStates->t + param->h;
	I_3 = currentStates->I + param->h * a3;
	X_3 = currentStates->X + param->h * b3;
	G_3 = currentStates->G + param->h * c3;
	a4 = (-param->n * I_3) + (param->tau * param->u);
	b4 = (-param->p2 * X_3) + (param->p3 * I_3);
	c4 = -(param->p1 + X_3)*currentStates->G + param->p1 * param->Gb + m(t_3, food);

	t_prox = currentStates->t + param->h;
	I_prox = currentStates->I + (param->h / 6.0) * (a1 + 2.0 * a2 + 2.0 * a3 + a4);
	X_prox = currentStates->X + (param->h / 6.0) * (b1 + 2.0 * b2 + 2.0 * b3 + b4);
	G_prox = currentStates->G + (param->h / 6.0) * (c1 + 2.0 * c2 + 2.0 * c3 + c4);

	// Guardar en la estructura los valores calculados
	currentStates->t = t_prox;
	currentStates->I = I_prox;
	currentStates->G = G_prox;
	currentStates->X = X_prox;

};

int main() {

	// Estructuras 
	struct states currentStates;
	struct parameters param;
	struct food_parameters food;

	double insulin[2001] = { 48.233 , 48.233 , 48.233 , 48.234 , 48.235 , 48.236 , 48.237 , 48.238 , 48.239 , 48.24 , 48.241 , 48.242 , 48.243 , 48.244 , 48.245 , 48.246 , 48.247 , 48.249 , 48.25 , 48.251 , 48.252 , 48.253 , 48.254 , 48.255 , 48.256 , 48.257 , 48.258 , 48.259 , 48.26 , 48.261 , 48.262 , 48.264 , 48.265 , 48.266 , 48.267 , 48.268 , 48.269 , 48.269 , 48.27 , 48.271 , 48.272 , 48.273 , 48.274 , 48.275 , 48.276 , 48.277 , 48.278 , 48.279 , 48.279 , 48.28 , 48.281 , 48.282 , 48.283 , 48.283 , 48.284 , 48.285 , 48.286 , 48.286 , 48.287 , 48.288 , 48.288 , 48.289 , 48.289 , 48.29 , 48.291 , 48.291 , 48.292 , 48.292 , 48.293 , 48.293 , 48.294 , 48.294 , 48.295 , 48.295 , 48.296 , 48.296 , 48.297 , 48.297 , 48.298 , 48.298 , 48.298 , 48.299 , 48.299 , 48.299 , 48.3 , 48.3 , 48.3 , 48.3 , 48.301 , 48.301 , 48.301 , 48.301 , 48.302 , 48.302 , 48.302 , 48.302 , 48.302 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.304 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.303 , 48.302 , 48.302 , 48.302 , 48.302 , 48.302 , 48.302 , 48.302 , 48.301 , 48.301 , 48.301 , 48.301 , 48.301 , 48.3 , 48.3 , 48.3 , 48.3 , 48.3 , 48.299 , 48.299 , 48.299 , 48.299 , 48.298 , 48.298 , 48.298 , 48.297 , 48.297 , 48.297 , 48.297 , 48.296 , 48.296 , 48.296 , 48.295 , 48.295 , 48.295 , 48.295 , 48.294 , 48.294 , 48.294 , 48.293 , 48.293 , 48.293 , 48.292 , 48.292 , 48.292 , 48.291 , 48.291 , 48.291 , 48.29 , 48.29 , 48.29 , 48.289 , 48.289 , 48.289 , 48.288 , 48.288 , 48.287 , 48.287 , 48.287 , 48.286 , 48.286 , 48.286 , 48.285 , 48.285 , 48.285 , 48.284 , 48.284 , 48.283 , 48.283 , 48.283 , 48.282 , 48.282 , 48.282 , 48.281 , 48.281 , 48.28 , 48.28 , 48.28 , 48.279 , 48.279 , 48.279 , 48.278 , 48.278 , 48.277 , 48.277 , 48.277 , 48.276 , 48.276 , 48.275 , 48.275 , 48.275 , 48.274 , 48.274 , 48.274 , 48.273 , 48.273 , 48.272 , 48.272 , 48.272 , 48.271 , 48.271 , 48.271 , 48.27 , 48.27 , 48.269 , 48.269 , 48.269 , 48.268 , 48.268 , 48.268 , 48.267 , 48.267 , 48.267 , 48.266 , 48.266 , 48.266 , 48.265 , 48.265 , 48.264 , 48.264 , 48.264 , 48.263 , 48.263 , 48.263 , 48.262 , 48.262 , 48.262 , 48.261 , 48.261 , 48.261 , 48.26 , 48.26 , 48.26 , 48.259 , 48.259 , 48.259 , 48.258 , 48.258 , 48.258 , 48.257 , 48.257 , 48.257 , 48.256 , 48.256 , 48.256 , 48.256 , 48.255 , 48.255 , 48.255 , 48.254 , 48.254 , 48.254 , 48.253 , 48.253 , 48.253 , 48.253 , 48.252 , 48.252 , 48.252 , 48.251 , 48.251 , 48.251 , 48.251 , 48.25 , 48.25 , 48.25 , 48.25 , 48.249 , 48.249 , 48.249 , 48.249 , 48.248 , 48.248 , 48.248 , 48.249 , 48.264 , 48.307 , 48.385 , 48.503 , 48.662 , 48.865 , 49.11 , 49.397 , 49.728 , 50.1 , 50.515 , 50.971 , 51.468 , 52.005 , 52.583 , 53.2 , 53.856 , 54.551 , 55.284 , 56.054 , 56.861 , 57.704 , 58.582 , 59.494 , 60.441 , 61.421 , 62.432 , 63.475 , 64.549 , 65.652 , 66.784 , 67.944 , 69.131 , 70.343 , 71.581 , 72.843 , 74.128 , 75.435 , 76.763 , 78.112 , 79.48 , 80.866 , 82.269 , 83.689 , 85.124 , 86.573 , 88.036 , 89.511 , 90.998 , 92.496 , 94.003 , 95.519 , 97.042 , 98.573 , 100.11 , 101.65 , 103.2 , 104.75 , 106.3 , 107.85 , 109.4 , 110.96 , 112.51 , 114.06 , 115.61 , 117.16 , 118.7 , 120.24 , 121.77 , 123.3 , 124.82 , 126.34 , 127.84 , 129.34 , 130.83 , 132.31 , 133.77 , 135.23 , 136.68 , 138.11 , 139.54 , 140.95 , 142.35 , 143.73 , 145.1 , 146.45 , 147.8 , 149.12 , 150.43 , 151.73 , 153.01 , 154.27 , 155.51 , 156.74 , 157.96 , 159.15 , 160.33 , 161.49 , 162.63 , 163.75 , 164.86 , 165.95 , 167.02 , 168.07 , 169.1 , 170.11 , 171.11 , 172.08 , 173.04 , 173.98 , 174.9 , 175.8 , 176.68 , 177.55 , 178.39 , 179.21 , 180.02 , 180.81 , 181.58 , 182.33 , 183.06 , 183.77 , 184.47 , 185.15 , 185.81 , 186.45 , 187.07 , 187.67 , 188.26 , 188.83 , 189.38 , 189.92 , 190.43 , 190.93 , 191.42 , 191.88 , 192.33 , 192.77 , 193.19 , 193.59 , 193.97 , 194.34 , 194.69 , 195.03 , 195.36 , 195.66 , 195.96 , 196.23 , 196.5 , 196.75 , 196.98 , 197.2 , 197.41 , 197.6 , 197.78 , 197.95 , 198.1 , 198.24 , 198.37 , 198.48 , 198.58 , 198.67 , 198.75 , 198.81 , 198.87 , 198.91 , 198.94 , 198.96 , 198.96 , 198.96 , 198.94 , 198.92 , 198.88 , 198.84 , 198.78 , 198.72 , 198.64 , 198.55 , 198.46 , 198.35 , 198.24 , 198.11 , 197.98 , 197.84 , 197.69 , 197.53 , 197.36 , 197.19 , 197.01 , 196.81 , 196.61 , 196.41 , 196.19 , 195.97 , 195.74 , 195.5 , 195.26 , 195.01 , 194.75 , 194.49 , 194.21 , 193.94 , 193.65 , 193.36 , 193.07 , 192.76 , 192.45 , 192.14 , 191.82 , 191.49 , 191.16 , 190.82 , 190.48 , 190.14 , 189.78 , 189.43 , 189.06 , 188.7 , 188.32 , 187.95 , 187.57 , 187.18 , 186.79 , 186.4 , 186 , 185.6 , 185.19 , 184.78 , 184.36 , 183.95 , 183.52 , 183.1 , 182.67 , 182.24 , 181.8 , 181.36 , 180.92 , 180.47 , 180.02 , 179.57 , 179.12 , 178.66 , 178.2 , 177.74 , 177.27 , 176.8 , 176.33 , 175.86 , 175.38 , 174.9 , 174.42 , 173.94 , 173.45 , 172.97 , 172.48 , 171.99 , 171.49 , 171 , 170.5 , 170 , 169.5 , 169 , 168.5 , 167.99 , 167.48 , 166.98 , 166.47 , 165.95 , 165.44 , 164.93 , 164.41 , 163.9 , 163.38 , 162.86 , 162.34 , 161.82 , 161.3 , 160.78 , 160.25 , 159.73 , 159.2 , 158.68 , 158.15 , 157.62 , 157.1 , 156.57 , 156.04 , 155.51 , 154.98 , 154.45 , 153.92 , 153.39 , 152.86 , 152.32 , 151.79 , 151.26 , 150.73 , 150.19 , 149.66 , 149.13 , 148.59 , 148.06 , 147.53 , 147 , 146.46 , 145.93 , 145.4 , 144.86 , 144.33 , 143.8 , 143.27 , 142.74 , 142.2 , 141.67 , 141.14 , 140.61 , 140.08 , 139.55 , 139.02 , 138.49 , 137.96 , 137.44 , 136.91 , 136.38 , 135.86 , 135.33 , 134.81 , 134.28 , 133.76 , 133.24 , 132.71 , 132.19 , 131.67 , 131.15 , 130.63 , 130.11 , 129.6 , 129.08 , 128.56 , 128.05 , 127.54 , 127.02 , 126.51 , 126 , 125.49 , 124.98 , 124.47 , 123.97 , 123.46 , 122.96 , 122.45 , 121.95 , 121.45 , 120.95 , 120.45 , 119.95 , 119.46 , 118.96 , 118.47 , 117.98 , 117.48 , 116.99 , 116.51 , 116.02 , 115.53 , 115.05 , 114.56 , 114.08 , 113.6 , 113.12 , 112.64 , 112.17 , 111.69 , 111.22 , 110.75 , 110.27 , 109.81 , 109.34 , 108.87 , 108.41 , 107.94 , 107.48 , 107.02 , 106.56 , 106.1 , 105.65 , 105.2 , 104.74 , 104.29 , 103.84 , 103.4 , 102.95 , 102.51 , 102.06 , 101.62 , 101.18 , 100.75 , 100.31 , 99.875 , 99.442 , 99.012 , 98.583 , 98.155 , 97.73 , 97.306 , 96.883 , 96.463 , 96.044 , 95.628 , 95.212 , 94.799 , 94.387 , 93.978 , 93.57 , 93.164 , 92.759 , 92.357 , 91.956 , 91.557 , 91.16 , 90.765 , 90.371 , 89.98 , 89.59 , 89.202 , 88.816 , 88.432 , 88.05 , 87.669 , 87.291 , 86.914 , 86.539 , 86.166 , 85.795 , 85.426 , 85.059 , 84.693 , 84.33 , 83.968 , 83.609 , 83.251 , 82.895 , 82.541 , 82.189 , 81.839 , 81.491 , 81.144 , 80.8 , 80.458 , 80.117 , 79.779 , 79.442 , 79.107 , 78.774 , 78.443 , 78.114 , 77.787 , 77.462 , 77.139 , 76.818 , 76.499 , 76.181 , 75.866 , 75.552 , 75.241 , 74.931 , 74.623 , 74.318 , 74.014 , 73.712 , 73.412 , 73.114 , 72.818 , 72.523 , 72.231 , 71.941 , 71.652 , 71.366 , 71.081 , 70.798 , 70.518 , 70.239 , 69.962 , 69.687 , 69.413 , 69.142 , 68.873 , 68.605 , 68.34 , 68.076 , 67.814 , 67.555 , 67.297 , 67.04 , 66.786 , 66.534 , 66.283 , 66.035 , 65.788 , 65.543 , 65.3 , 65.059 , 64.82 , 64.582 , 64.347 , 64.113 , 63.881 , 63.651 , 63.423 , 63.196 , 62.972 , 62.749 , 62.528 , 62.309 , 62.091 , 61.876 , 61.662 , 61.45 , 61.24 , 61.031 , 60.825 , 60.62 , 60.417 , 60.215 , 60.016 , 59.818 , 59.622 , 59.427 , 59.235 , 59.044 , 58.854 , 58.667 , 58.481 , 58.297 , 58.115 , 57.934 , 57.755 , 57.578 , 57.402 , 57.228 , 57.056 , 56.885 , 56.716 , 56.548 , 56.383 , 56.219 , 56.056 , 55.895 , 55.736 , 55.578 , 55.422 , 55.268 , 55.115 , 54.963 , 54.814 , 54.665 , 54.519 , 54.374 , 54.23 , 54.088 , 53.947 , 53.808 , 53.671 , 53.535 , 53.4 , 53.267 , 53.136 , 53.006 , 52.877 , 52.75 , 52.624 , 52.5 , 52.377 , 52.256 , 52.136 , 52.017 , 51.9 , 51.784 , 51.67 , 51.557 , 51.445 , 51.335 , 51.226 , 51.119 , 51.013 , 50.908 , 50.804 , 50.702 , 50.601 , 50.501 , 50.403 , 50.306 , 50.21 , 50.116 , 50.022 , 49.93 , 49.84 , 49.75 , 49.662 , 49.575 , 49.489 , 49.404 , 49.321 , 49.239 , 49.157 , 49.078 , 48.999 , 48.921 , 48.845 , 48.769 , 48.695 , 48.622 , 48.55 , 48.479 , 48.409 , 48.341 , 48.273 , 48.207 , 48.141 , 48.077 , 48.013 , 47.951 , 47.89 , 47.829 , 47.77 , 47.712 , 47.654 , 47.598 , 47.543 , 47.488 , 47.435 , 47.383 , 47.331 , 47.28 , 47.231 , 47.182 , 47.134 , 47.087 , 47.041 , 46.996 , 46.952 , 46.909 , 46.866 , 46.825 , 46.784 , 46.744 , 46.705 , 46.667 , 46.629 , 46.592 , 46.557 , 46.521 , 46.487 , 46.454 , 46.421 , 46.389 , 46.358 , 46.327 , 46.297 , 46.268 , 46.24 , 46.213 , 46.186 , 46.16 , 46.134 , 46.109 , 46.085 , 46.062 , 46.039 , 46.017 , 45.995 , 45.975 , 45.954 , 45.935 , 45.916 , 45.897 , 45.88 , 45.863 , 45.846 , 45.83 , 45.815 , 45.8 , 45.785 , 45.772 , 45.759 , 45.746 , 45.734 , 45.722 , 45.711 , 45.701 , 45.691 , 45.681 , 45.672 , 45.664 , 45.655 , 45.648 , 45.641 , 45.634 , 45.628 , 45.622 , 45.617 , 45.612 , 45.607 , 45.603 , 45.6 , 45.597 , 45.594 , 45.591 , 45.589 , 45.588 , 45.587 , 45.586 , 45.585 , 45.585 , 45.585 , 45.586 , 45.587 , 45.588 , 45.59 , 45.592 , 45.594 , 45.596 , 45.599 , 45.603 , 45.606 , 45.61 , 45.614 , 45.618 , 45.623 , 45.628 , 45.633 , 45.639 , 45.644 , 45.65 , 45.657 , 45.663 , 45.67 , 45.677 , 45.684 , 45.691 , 45.699 , 45.707 , 45.715 , 45.723 , 45.732 , 45.741 , 45.749 , 45.759 , 45.768 , 45.777 , 45.787 , 45.797 , 45.807 , 45.817 , 45.827 , 45.838 , 45.848 , 45.859 , 45.87 , 45.881 , 45.892 , 45.904 , 45.915 , 45.927 , 45.939 , 45.95 , 45.962 , 45.975 , 45.987 , 45.999 , 46.011 , 46.024 , 46.037 , 46.049 , 46.062 , 46.075 , 46.088 , 46.101 , 46.114 , 46.127 , 46.141 , 46.154 , 46.167 , 46.181 , 46.195 , 46.208 , 46.222 , 46.235 , 46.249 , 46.263 , 46.277 , 46.291 , 46.305 , 46.319 , 46.333 , 46.347 , 46.361 , 46.375 , 46.389 , 46.403 , 46.417 , 46.431 , 46.446 , 46.46 , 46.474 , 46.488 , 46.503 , 46.517 , 46.531 , 46.545 , 46.56 , 46.574 , 46.588 , 46.602 , 46.616 , 46.631 , 46.645 , 46.659 , 46.673 , 46.687 , 46.701 , 46.716 , 46.73 , 46.744 , 46.758 , 46.772 , 46.786 , 46.8 , 46.814 , 46.828 , 46.841 , 46.855 , 46.869 , 46.883 , 46.896 , 46.91 , 46.924 , 46.937 , 46.951 , 46.964 , 46.978 , 46.991 , 47.005 , 47.018 , 47.031 , 47.044 , 47.057 , 47.071 , 47.084 , 47.097 , 47.11 , 47.122 , 47.135 , 47.148 , 47.161 , 47.173 , 47.186 , 47.198 , 47.211 , 47.223 , 47.236 , 47.248 , 47.26 , 47.272 , 47.284 , 47.296 , 47.308 , 47.32 , 47.332 , 47.344 , 47.355 , 47.367 , 47.378 , 47.39 , 47.401 , 47.413 , 47.424 , 47.435 , 47.446 , 47.457 , 47.468 , 47.479 , 47.49 , 47.5 , 47.511 , 47.522 , 47.532 , 47.543 , 47.553 , 47.563 , 47.573 , 47.583 , 47.594 , 47.604 , 47.613 , 47.623 , 47.633 , 47.643 , 47.652 , 47.662 , 47.671 , 47.681 , 47.69 , 47.699 , 47.708 , 47.717 , 47.726 , 47.735 , 47.744 , 47.753 , 47.761 , 47.77 , 47.778 , 47.787 , 47.795 , 47.804 , 47.812 , 47.82 , 47.828 , 47.836 , 47.844 , 47.852 , 47.859 , 47.867 , 47.875 , 47.882 , 47.89 , 47.897 , 47.904 , 47.912 , 47.919 , 47.926 , 47.933 , 47.94 , 47.947 , 47.954 , 47.96 , 47.967 , 47.973 , 47.98 , 47.986 , 47.993 , 47.999 , 48.005 , 48.011 , 48.018 , 48.024 , 48.03 , 48.035 , 48.041 , 48.047 , 48.053 , 48.058 , 48.064 , 48.069 , 48.075 , 48.08 , 48.085 , 48.09 , 48.096 , 48.101 , 48.106 , 48.111 , 48.116 , 48.12 , 48.125 , 48.13 , 48.134 , 48.139 , 48.144 , 48.148 , 48.152 , 48.157 , 48.161 , 48.165 , 48.169 , 48.173 , 48.178 , 48.181 , 48.185 , 48.189 , 48.193 , 48.197 , 48.201 , 48.204 , 48.208 , 48.211 , 48.215 , 48.218 , 48.222 , 48.225 , 48.228 , 48.231 , 48.234 , 48.238 , 48.241 , 48.244 , 48.247 , 48.249 , 48.252 , 48.255 , 48.258 , 48.261 , 48.263 , 48.266 , 48.268 , 48.271 , 48.273 , 48.276 , 48.278 , 48.281 , 48.283 , 48.285 , 48.287 , 48.29 , 48.292 , 48.294 , 48.296 , 48.298 , 48.3 , 48.302 , 48.304 , 48.305 , 48.307 , 48.309 , 48.311 , 48.312 , 48.314 , 48.316 , 48.317 , 48.319 , 48.32 , 48.322 , 48.323 , 48.324 , 48.326 , 48.327 , 48.328 , 48.33 , 48.331 , 48.332 , 48.333 , 48.334 , 48.335 , 48.336 , 48.337 , 48.338 , 48.339 , 48.34 , 48.341 , 48.342 , 48.343 , 48.344 , 48.345 , 48.345 , 48.346 , 48.347 , 48.347 , 48.348 , 48.349 , 48.349 , 48.35 , 48.35 , 48.351 , 48.351 , 48.352 , 48.352 , 48.353 , 48.353 , 48.354 , 48.354 , 48.354 , 48.355 , 48.355 , 48.355 , 48.355 , 48.356 , 48.356 , 48.356 , 48.356 , 48.356 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.357 , 48.356 , 48.356 , 48.356 , 48.356 , 48.356 , 48.356 , 48.355 , 48.355 , 48.355 , 48.355 , 48.354 , 48.354 , 48.354 , 48.354 , 48.353 , 48.353 , 48.353 , 48.352 , 48.352 , 48.352 , 48.351 , 48.351 , 48.35 , 48.35 , 48.35 , 48.349 , 48.349 , 48.348 , 48.348 , 48.347 , 48.347 , 48.347 , 48.346 , 48.346 , 48.345 , 48.345 , 48.344 , 48.344 , 48.343 , 48.343 , 48.342 , 48.341 , 48.341 , 48.34 , 48.34 , 48.339 , 48.339 , 48.338 , 48.338 , 48.337 , 48.336 , 48.336 , 48.335 , 48.335 , 48.334 , 48.333 , 48.333 , 48.332 , 48.332 , 48.331 , 48.33 , 48.33 , 48.329 , 48.328 , 48.328 , 48.327 , 48.327 , 48.326 , 48.325 , 48.325 , 48.324 , 48.323 , 48.323 , 48.322 , 48.321 , 48.321 , 48.32 , 48.319 , 48.319 , 48.318 , 48.317 , 48.317 , 48.316 , 48.315 , 48.315 , 48.314 , 48.313 , 48.313 , 48.312 , 48.311 , 48.311 , 48.31 , 48.309 , 48.309 , 48.308 , 48.307 , 48.307 , 48.306 , 48.305 , 48.305 , 48.304 , 48.303 , 48.303 , 48.302 , 48.301 , 48.301 , 48.3 , 48.3 , 48.299 , 48.298 , 48.298 , 48.297 , 48.296 , 48.296 , 48.295 , 48.294 , 48.294 , 48.293 , 48.292 , 48.292 , 48.291 , 48.291 , 48.29 , 48.289 , 48.289 , 48.288 , 48.288 , 48.287 , 48.286 , 48.286 , 48.285 , 48.284 , 48.284 , 48.283 , 48.283 , 48.282 , 48.282 , 48.281 , 48.28 , 48.28 , 48.279 , 48.279 , 48.278 , 48.278 , 48.277 , 48.276 , 48.276 , 48.275 , 48.275 , 48.274 , 48.274 , 48.273 , 48.273 , 48.272 , 48.272 , 48.271 , 48.27 , 48.27 , 48.269 , 48.269 , 48.268 , 48.268 , 48.267 , 48.267 , 48.266 , 48.266 , 48.265 , 48.265 , 48.264 , 48.264 , 48.263 , 48.263 , 48.263 , 48.262 , 48.262 , 48.261 , 48.261 , 48.26 , 48.26 , 48.259 , 48.259 , 48.259 , 48.258 , 48.258 , 48.257 , 48.257 , 48.256 , 48.256 , 48.256 , 48.255 , 48.255 , 48.254 , 48.254 , 48.254 , 48.253 , 48.253 , 48.252 , 48.252 , 48.252 , 48.251 , 48.251 , 48.251 , 48.25 , 48.25 , 48.249 , 48.249 , 48.249 , 48.248 , 48.248 , 48.248 , 48.247 , 48.247 , 48.247 , 48.246 , 48.246 , 48.246 , 48.245 , 48.245 , 48.245 , 48.245 , 48.244 , 48.244 , 48.244 , 48.243 , 48.243 , 48.243 , 48.243 , 48.242 , 48.242 , 48.242 , 48.241 , 48.241 , 48.241 , 48.241 , 48.24 , 48.24 , 48.24 , 48.24 , 48.239 , 48.239 , 48.239 , 48.239 , 48.238 , 48.238 , 48.238 , 48.238 , 48.238 , 48.237 , 48.237 , 48.237 , 48.237 , 48.236 , 48.236 , 48.236 , 48.236 , 48.236 , 48.235 , 48.235 , 48.235 , 48.235 , 48.235 , 48.235 , 48.234 , 48.234 , 48.234 , 48.234 , 48.234 , 48.234 , 48.233 , 48.233 , 48.233 , 48.233 , 48.233 , 48.233 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.227 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.228 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.229 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.23 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.231 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232 , 48.232};
	
	// Condiciones iniciales
	currentStates.I = 18.7813;
	currentStates.X = 0.0067;
	currentStates.G = 120.0;
	currentStates.t = 0.0;

	// Parametros del paciente
	param.p1 = 0.0337;
	param.p2 = 0.0209;
	param.p3 = 7.5 * pow(10, -6);
	param.tau = 0.083333;
	param.n = 0.214;
	param.Gb = 144.0;
	param.h = 1.0;

	// Parametros de una comida
	food.Dg = 60.0;
	food.Ag = 0.8;
	food.tmax_I = 55.0;
	food.tmax_G = 40.0;
	food.tiempo_food = 300.0;
	food.Vg = 13.79;
	

	int x = 0;

	FILE *f = fopen("rk_gluc.csv", "w");
	if (f == NULL) return -1;

	FILE *f2 = fopen("rk_ctrl.csv", "w");
	if (f2 == NULL) return -1;

	for (x; x < 2001; x++) {
		param.u = insulin[x];
		printf("%lf\n", currentStates.G);
		fprintf(f, "%lf\n", currentStates.G);
		fprintf(f2, "%lf\n", param.u);
		//eulerSolver(&param, &currentStates, &food);
		rungeKutta4Solver(&param, &currentStates, &food);
		};

	fclose(f);
	return 0;
};
