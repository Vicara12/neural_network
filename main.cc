#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include "NeuralNetwork.hh"
#include "NeuralTrainer.hh"
using namespace std;

int main ()
{
   vector<unsigned> topology({2, 2,1});
   vector<string> activation_functions({"tanh", "tanh", "tanh"});
   NeuralNetworkParameters netParams(topology, activation_functions, "mse");
   netParams.batchs_for_avg_error = 1;

   NeuralNetwork net(netParams);

   vector<vector<double>> input = {{0,0}, {0,1}, {1,0}, {1,1}};
   vector<vector<double>> target = {{0}, {1}, {1}, {0}};

   int times;
   double learning_rate;

   cout << "input cycles and learning rate: ";
   cin >> times >> learning_rate;

   NeuralTrainer::basicTraining(net, input, target, times, learning_rate, true, true);

   net.draw();

   cout << "\n FINISHED TRAINNING\n INSERT YOUR INPUT\n\n";

   vector<double> user_input(2);

   while (cin >> user_input[0] >> user_input[1])
   {
      vector<double> output(1);
      net.compute(user_input);
      net.getOutput(output);
      cout << output[0] << endl;
   }
}