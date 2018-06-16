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



struct states {


	double x1;


	double x2;


	double x3;


};



double lqrController(struct matG *G, struct matH *H, struct matC *C, struct matKe *Ke, struct matK1 *K1, struct matK2 *K2, struct states *x, double r_act, double y_act, double *I_past) {


	double I_act, ctrlsig_u, x1_next, x2_next, x3_next;


	// Valores actuales


	I_act = *I_past + r_act - y_act;


	ctrlsig_u = K1->K1_11 * I_act - (K2->K2_11 * x->x1 + K2->K2_12 * x->x2 + K2->K2_13 * x->x3);


	// Valores siguientes


	x1_next = G->G11 * x->x1 + G->G12 * x->x2 + G->G13 * x->x3 + H->H11 * ctrlsig_u + Ke->Ke_11 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);


	x2_next = G->G21 * x->x1 + G->G22 * x->x2 + G->G23 * x->x3 + H->H21 * ctrlsig_u + Ke->Ke_21 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);


	x3_next = G->G31 * x->x1 + G->G32 * x->x2 + G->G33 * x->x3 + H->H31 * ctrlsig_u + Ke->Ke_31 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);



	return ctrlsig_u;


};



int main() {



	//Inicializacion de matrices del controlador


	struct matG G;


	struct matH H;


	struct matC C;


	struct matKe Ke;


	struct matK1 K1;


	struct matK2 K2;


	struct states currentStates;



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



	//Inicializacion de otros parametros 


	double r_act = 120.0;


	double y_act = 270.0;


	double I_past = 0.0;


	double ctrlsig_u = 0.0;



	ctrlsig_u = lqrController(&G, &H, &C, &Ke, &K1, &K2, &currentStates, r_act, y_act, &I_past);


	printf("Ctrl signal: %lf\n", ctrlsig_u);



　


	return 0;


};
