#include "Neuron.hh"

Neuron::Neuron (std::string act_func_name,
                int n_inputs,
                double max_rand,
                int index)
{
   weight = std::vector<Connection>(n_inputs, Connection(0));
   act_func = initActFunc(act_func_name);
   bias = false;
   my_index = index;
   last_output = 0;
   gradient = 0;

   for (int i = 0; i < n_inputs; i++)
      weight[i].weight = getRandomWeight(max_rand);
}

Neuron::Neuron (std::string act_func_name,
                std::vector<double> weights,
                int index)
{
   for (int i = 0; i < weight.size(); i++)
      weight[i].weight = weights[i];

   act_func = initActFunc(act_func_name);
   bias = false;
   my_index = index;
   last_output = 0;
   gradient = 0;
}

Neuron::Neuron (double bias_output, int index) : last_bias_actualization(0)
{
   last_output = bias_output;
   bias = true;
   my_index = index;
}

void Neuron::calculateOutput (const Layer& previous_layer)
{
   if (bias)
      throw BiasCalculateOutputError();

   last_output = 0;

   for (int i = 0; i < previous_layer.size(); i++)
      last_output += previous_layer[i].getOutput() * weight[i].weight;

   last_output = act_func->f(last_output);
}

double Neuron::getOutput () const
{
   return last_output;
}

void Neuron::setOutput (double output)
{
   last_output = output;
}

void Neuron::calculateGradient (const Layer& next_layer)
{
   gradient = 0;

   for (int i = 0; i < next_layer.size() - 1; i++)
      gradient += next_layer[i].weight[my_index].weight * next_layer[i].gradient;
   
   if (not bias)
      gradient *= act_func->df(last_output);
}

void Neuron::setGradient (double new_gradient)
{
   gradient = new_gradient;

   if (not bias)
      gradient *= act_func->df(last_output);
}

void Neuron::actualize (const Layer& previous_layer,
                        double learning_rate,
                        double inertia)
{
   double delta;

   if (bias)
   {
      delta = learning_rate * gradient +
                      inertia * last_bias_actualization;

      last_output += delta;
      last_bias_actualization = delta;
   }
   else
   {
      for (int i = 0; i < weight.size(); i++)
      {
         delta = learning_rate * previous_layer[i].last_output * gradient +
                 inertia * weight[i].weight_gradient;
         
         weight[i].weight += delta;
         weight[i].weight_gradient = delta;
      }
   }
}

std::string Neuron::activationFunctionName () const
{
   return act_func->getName();
}

void Neuron::draw () const
{
   if (bias)
      std::cout << "(" << last_output << ")";
   else
   {
      for (int i = 0; i < weight.size(); i++)
      {
         if (i != 0)
            std::cout << " ";
         std::cout << weight[i].weight;
      }
   }
   std::cout << std::endl;
}

ActivationFunction* Neuron::initActFunc (std::string act_func_name)
{
   if (act_func_name == "sigmoid")  return Sigmoid::getInstance();
   else if (act_func_name == "tanh")  return Tanh::getInstance();
   else if (act_func_name == "linear")  return Linear::getInstance();
   else if (act_func_name == "relu")  return ReLu::getInstance();
   else  throw ActivationFunctionNotFound();

   return NULL;
}

double Neuron::getRandomWeight (double max_rand)
{
   return max_rand * rand() / double(RAND_MAX);
}

std::vector<double> Neuron::getWeights () const
{
   std::vector<double> result(weight.size());

   for (int i = 0; i < weight.size(); i++)
      result[i] = weight[i].weight;

   return result;
}