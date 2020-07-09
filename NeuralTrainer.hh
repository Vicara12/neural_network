#ifndef NEURALTRAINER_HH_
#define NEURALTRAINER_HH_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <exception>
#include "NeuralNetwork.hh"

typedef std::vector<std::vector<double>> Data;

class NeuralTrainer
{
public:
   static double basicTraining (NeuralNetwork& net,
                                Data input,
                                Data target,
                                int times,
                                double learning_rate,
                                bool show_progress = false);

   static std::pair<double, double> normalTraining (NeuralNetwork& net,
                                                    Data input,
                                                    Data target,
                                                    int times,
                                                    double learning_rate,
                                                    double test_percentage,
                                                    bool show_progress = false);

private:
   NeuralTrainer () {}
};


/*
 *    EXCEPTIONS
 */

struct InputAndOutputSizesDoNotMAtch : public std::exception
{
  const char* what () const throw ()
  {
    return "the size of the input and output vectors don't match";
  }
};

struct InvalidTimes : public std::exception
{
  const char* what () const throw ()
  {
    return "the value of times must be greater than 0";
  }
};

struct TestPercentageIsTooSmall : public std::exception
{
  const char* what () const throw ()
  {
    return "the test percentage must include at least one test sample";
  }
};

#endif