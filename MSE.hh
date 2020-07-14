#ifndef MSE_HH_
#define MSE_HH_

#include <malloc.h>
#include "CostFunction.hh"

class MSE : public CostFunction
{
public:
   MSE (bool normalize_output);
   std::string getName () const;
   double error (const Layer& output, const vec_double& target) const;
   void dError (Layer& output, const vec_double& target) const;
};

#endif