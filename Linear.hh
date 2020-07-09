#ifndef LINEAR_HH_
#define LINEAR_HH_

#include "ActivationFunction.hh"

class Linear : public ActivationFunction
{
public:
   static ActivationFunction* getInstance ();
   std::string getName () const;
   double f (double x) const;
   double df (double x) const;
private:
   Linear ();
   static ActivationFunction* instance;
};

#endif