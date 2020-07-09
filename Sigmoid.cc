#include "Sigmoid.hh"

ActivationFunction* Sigmoid::instance = new Sigmoid();

Sigmoid::Sigmoid () {};

ActivationFunction* Sigmoid::getInstance ()
{
   return Sigmoid::instance;
}

std::string Sigmoid::getName () const
{
   return "sigmoid";
}

double Sigmoid::f (double x) const
{
   return 1.0 / (1 + pow(E_EULER, -x));
}

double Sigmoid::df (double x) const
{
   double e_to_x = pow(E_EULER, x);
   return e_to_x / ((e_to_x+1) * (e_to_x+1));
}