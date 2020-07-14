#ifndef COSTFUNCTION_HH_
#define COSTFUNCTION_HH_

#include <vector>
#include <exception>
#include "Neuron.hh"

typedef std::vector<double> vec_double;

class CostFunction
{
public:
  CostFunction(bool normalize_output) : normalized_output(normalize_output) {}
  virtual std::string getName () const = 0;
  virtual double error (const Layer& output, const vec_double& target) const = 0;
  virtual void dError (Layer& output, const vec_double& target) const = 0;

protected:
   bool normalized_output;
};

struct InvalidTargetSize : public std::exception
{
  const char* what () const throw ()
  {
    return "last layer's size and target values' size do not match";
  }
};

#endif