#include "Tanh.hh"

ActivationFunction* Tanh::instance = new Tanh();

Tanh::Tanh () {}

ActivationFunction* Tanh::getInstance ()
{
   return instance;
}

std::string Tanh::getName () const
{
   return "tanh";
}

double Tanh::f (double x) const
{
   return tanh(x);
}

double Tanh::df (double x) const
{
   return 1.0 - x*x;
}