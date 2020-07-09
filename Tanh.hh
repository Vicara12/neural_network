#ifndef TANH_HH_
#define TANH_HH_

#include "ActivationFunction.hh"
#include <math.h>

class Tanh : public ActivationFunction
{
public:
   static ActivationFunction* getInstance ();
   std::string getName () const;
   double f (double x) const;
   double df (double x) const;
private:
   Tanh ();
   static ActivationFunction* instance;
};

#endif