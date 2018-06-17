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
	ctrlsig_u = K1->K1_11 * I_act - (K2->K2_11 * x->x1 + K2->K2_12 * x->x2 + K2->K2_13 * x->x3);

	// Valores siguientes
	x1_next = G->G11 * x->x1 + G->G12 * x->x2 + G->G13 * x->x3 + H->H11 * ctrlsig_u + Ke->Ke_11 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);
	x2_next = G->G21 * x->x1 + G->G22 * x->x2 + G->G23 * x->x3 + H->H21 * ctrlsig_u + Ke->Ke_21 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);
	x3_next = G->G31 * x->x1 + G->G32 * x->x2 + G->G33 * x->x3 + H->H31 * ctrlsig_u + Ke->Ke_31 * (y_act - C->C11 * x->x1 - C->C12 * x->x2 - C->C13 * x->x3);

	return ctrlsig_u;
}
