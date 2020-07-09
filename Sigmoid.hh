#ifndef SIGMOID_HH_
#define SIGMOID_HH_

#include "ActivationFunction.hh"
#include "math.h"

#define E_EULER 2.71828182845904523

class Sigmoid : public ActivationFunction
{
public:
   static ActivationFunction* getInstance ();
   std::string getName () const;
   double f (double x) const;
   double df (double x) const;
private:
   Sigmoid ();
   static ActivationFunction* instance;
};

#endif