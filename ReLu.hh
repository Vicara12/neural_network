#ifndef RELU_HH_
#define RELU_HH_

#include "ActivationFunction.hh"

class ReLu : public ActivationFunction
{
public:
   static ActivationFunction* getInstance ();
   std::string getName () const;
   double f (double x) const;
   double df (double x) const;
private:
   ReLu ();
   static ActivationFunction* instance;
};

#endif