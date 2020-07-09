#ifndef ACTIVATIONFUNCTION_HH_
#define ACTIVATIONFUNCTION_HH_

#include <iostream>

class ActivationFunction
{
public:
   ActivationFunction () {}
   virtual std::string getName () const = 0;
   virtual double f (double x) const = 0;
   virtual double df (double x) const = 0;
};

#endif