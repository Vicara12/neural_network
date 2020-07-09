#include "ReLu.hh"

ActivationFunction* ReLu::instance = new ReLu();

ReLu::ReLu () {}

ActivationFunction* ReLu::getInstance ()
{
   return instance;
}

std::string ReLu::getName () const
{
   return "relu";
}

double ReLu::f (double x) const
{
   return x < 0 ? 0 : x;
}

double ReLu::df (double x) const
{
   return x >= 0;
}