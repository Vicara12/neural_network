#include "Linear.hh"

ActivationFunction* Linear::instance = new Linear();

Linear::Linear () {}

ActivationFunction* Linear::getInstance ()
{
   return instance;
}

std::string Linear::getName () const
{
   return "linear";
}

double Linear::f (double x) const
{
   return x;
}

double Linear::df (double x) const
{
   return x-x;
}