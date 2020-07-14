#include "MSE.hh"

MSE::MSE (bool normalize_output) : CostFunction(normalize_output)
{

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

   if (normalized_output)
   {
      double output_sum = 0;

      for (auto neuron : output)
         output_sum += neuron.getOutput();
      
      output_sum -= output.back().getOutput();
      
      int out_size = output.size() - 1;
      
      double default_out = 1.0/out_size;

      if (output_sum != 0)
         for (int i = 0; i < out_size; i++)
            error += pow(target[i] - output[i].getOutput()/output_sum, 2);
      else
         for (int i = 0; i < out_size; i++)
            error += pow(target[i] - default_out, 2);
   }
   else
      for (int i = 0; i < target.size(); i++)
         error += pow(target[i] - output[i].getOutput(), 2);
   
   return sqrt(error);
}


void MSE::dError (Layer& output, const vec_double& target) const
{
   if (output.size()-1 != target.size())
      throw InvalidTargetSize(); 

   if (normalized_output)
   {
      double output_sum = 0;

      for (auto neuron : output)
         output_sum += neuron.getOutput();
      
      output_sum -= output.back().getOutput();
      
      int out_size = output.size() - 1;
      
      double default_out = 1.0/out_size;
      double out_sum_sqrd = output_sum * output_sum;

      for (int i = 0; i < out_size; i++)
      {
         double af_gradient = output_sum != 0 ? 
                              target[i] - output[i].getOutput()/output_sum :
                              target[i] - default_out;
         
         double neuron_gradient = output_sum != 0 ?
                           (output_sum - output[i].getOutput())/out_sum_sqrd :
                           1;
         
         output[i].setGradient(af_gradient * neuron_gradient);
      }
   }
   else
      for (int i = 0; i < target.size(); i++)
         output[i].setGradient(target[i] - output[i].getOutput());
}