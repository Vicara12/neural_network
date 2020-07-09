#include "MSE.hh"

CostFunction* MSE::instance = new MSE();

MSE::MSE () {}

CostFunction* MSE::getInstance ()
{
   return instance;
}

std::string MSE::getName () const
{
   return "mse";
}

double MSE::error (const Layer& output, const vec_double& target) const
{
   if (output.size()-1 != target.size())
      throw InvalidTargetSize(); 

   double error = 0;

   for (int i = 0; i < target.size(); i++)
      error += pow(target[i] - output[i].getOutput(), 2);
   
   return sqrt(error);
}


void MSE::dError (Layer& output, const vec_double& target) const
{
   if (output.size()-1 != target.size())
      throw InvalidTargetSize(); 


   for (int i = 0; i < target.size(); i++)
      output[i].setGradient(target[i] - output[i].getOutput());
}