#ifndef __MODELS_H
#define __MODELS_H

// Estados del modelo minimo de Bergman
struct states {
  double G;
  double I;
  double X;
  double t;
};

// Parametros del modelo minimo de Bergman
struct parameters {
  double p1;
  double p2;
  double p3;
  double tau;
  double n;
  double Gb;
  double u;
  double h;
};

// Parametros de la funcion de comida
struct food_parameters {
  double Dg;
  double Ag;
  double tmax_I;
  double tmax_G;
  double time_food;
  double Vg;

};

#endif
