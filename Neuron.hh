#ifndef NEURON_HH_
#define NEURON_HH_

#include <vector>
#include <exception>
#include <iostream>
#include <stdlib.h>
#include "Sigmoid.hh"
#include "Tanh.hh"

class Neuron;

typedef std::vector<Neuron> Layer;

class Neuron
{
public:
   Neuron () {}
   Neuron (std::string act_func_name, int n_inputs, double max_rand, int index);
   Neuron (std::string act_func_name, std::vector<double> weights, int index);
   Neuron (double bias_output, int index);
   void calculateOutput (const Layer& previous_layer);
   double getOutput () const;
   void setOutput (double output);
   void calculateGradient (const Layer& next_layer);
   void setGradient (double new_gradient);
   void actualize (const Layer& previous_layer, double learning_rate);
   std::string activationFunctionName () const;
   void draw () const;
   std::vector<double> getWeights () const;

private:
   static ActivationFunction* initActFunc (std::string act_func_name);
   static double getRandomWeight (double max_rand);

   std::vector<double> weight;
   ActivationFunction* act_func;
   int my_index;
   double last_output;
   double gradient;
   bool bias;
};

/*
 *    POSSIBLE EXCEPTIONS
 */

struct ActivationFunctionNotFound : public std::exception
{
  const char* what () const throw ()
  {
    return "the selected activation function could not be found";
  }
};

struct BiasCalculateOutputError : public std::exception
{
  const char* what () const throw ()
  {
    return "bias neurons can not compute an output";
  }
};


#endif