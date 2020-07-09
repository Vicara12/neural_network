#ifndef NEURALNETWORK_HH_
#define NEURALNETWORK_HH_

#include <exception>
#include <vector>
#include <iostream>
#include <list>
#include <time.h>
#include <fstream>
#include "Neuron.hh"
#include "MSE.hh"

struct NeuralNetworkParameters
{
   std::vector<unsigned int> topology;
   std::vector<std::string> activation_functions;
   std::string cost_function;
   double weight_max;
   double bias_initial_value;
   double batchs_for_avg_error;

   NeuralNetworkParameters(std::vector<unsigned int> nn_topology,
                           std::vector<std::string> nn_activation_functions,
                           std::string nn_cost_function);
};


class NeuralNetwork
{
public:
   NeuralNetwork (const NeuralNetworkParameters& netData);
   NeuralNetwork (std::string filePath);
   void compute (const std::vector<double>& input);
   void getOutput (std::vector<double>& result);
   double error (const std::vector<double>& input,
                 const std::vector<double>& target);
   double avgError (const std::vector<double>& input,
                    const std::vector<double>& target);
   void clearAvgError ();
   void changeBatchsForAVGError (unsigned qty);
   double backPropagate (const std::vector<double>& input,
                         const std::vector<double>& target,
                         double learning_rate);
   void saveToFile (std::string filePath) const;
   void draw () const;
   unsigned int qtyInputs () const;
   unsigned int qtyOutputs () const;

private:
   CostFunction* initCostFunction (std::string cost_function_name);
   double computeAvgError (double this_error);

   std::vector<Layer> layer;
   CostFunction* cost_function;
   std::list<double> error_list;
   double total_error;
   int batchs_for_avg_error;
};

/*
 *    EXCEPTIONS
 */

struct InvalidTopology : public std::exception
{
  const char* what () const throw ()
  {
    return "topology must have more than one strictly positive integer";
  }
};

struct InvalidBatchErrorAVGQty : public std::exception
{
  const char* what () const throw ()
  {
    return "batchs for average error must be greater than 0";
  }
};

struct CostFunctionNotFound : public std::exception
{
  const char* what () const throw ()
  {
    return "the selected cost function does not exist";
  }
};

struct InvalidInputSize : public std::exception
{
  const char* what () const throw ()
  {
    return "the shape of the input is not equal to the number of input neurons";
  }
};

struct InvalidOutputSize : public std::exception
{
  const char* what () const throw ()
  {
    return "size of the output vector must match the number of output neurons";
  }
};

struct ErrorSavingNeuralNetwork : public std::exception
{
  const char* what () const throw ()
  {
    return "an error happened while exporting the neural network to a file";
  }
};

struct ErrorOpeningNeuralNetwork : public std::exception
{
  const char* what () const throw ()
  {
    return "error opening data file";
  }
};

struct ErrorLoadingNeuralNetwork : public std::exception
{
  const char* what () const throw ()
  {
    return "file has been modified in a way that makes it no longer valid";
  }
};

#endif