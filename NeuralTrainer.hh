#ifndef NEURALTRAINER_HH_
#define NEURALTRAINER_HH_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <exception>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include "NeuralNetwork.hh"

typedef std::vector<std::vector<double>> Data;

class NeuralTrainer
{
public:
   static double basicTraining (NeuralNetwork& net,
                                const Data& input,
                                const Data& target,
                                int times,
                                double learning_rate,
                                double inertia,
                                bool train_bias = true,
                                bool show_progress = false,
                                bool plot = false,
                                int ini = -1,
                                int fi = -1);

   static std::pair<double, double> normalTraining (NeuralNetwork& net,
                                                    const Data& input,
                                                    const Data& target,
                                                    int times,
                                                    double learning_rate,
                                                    double inertia,
                                                    double test_percentage,
                                                    bool train_bias = true,
                                                    bool show_progress = false,
                                                    bool plot = false);

  static double testClasifier (NeuralNetwork& net,
                               const Data& input,
                               const Data& target,
                               int ini = -1,
                               int fi = -1);

private:
   NeuralTrainer () {}
   static void plotError (const std::list<double>& error_list);
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

struct UnableToPlotError : public std::exception
{
  const char* what () const throw ()
  {
    return "there was an error when trying to plot the error";
  }
};

#endif