#include <iostream>
#include <vector>
#include <fstream>
#include "NeuralNetwork.hh"
#include "NeuralTrainer.hh"
using namespace std;

typedef vector<vector<double>> Data;


void getXOR (Data& input, Data& target)
{
   input = {{0,0}, {0,1}, {1,0}, {1,1}};
   target = {{0}, {1}, {1}, {0}};
}


void getMNIST (Data& input, Data& target)
{
   input.clear();
   target.clear();

   fstream file;
   file.open("./datasets/mnist.csv", ios::in);

   if (not file)
   {
      cout << "error opening file" << endl;
      return;
   }

   while (not file.eof())
   {      
      int number;
      target.push_back(vector<double>(10, 0));
      file >> number;
      target.back()[number] = 1;

      input.push_back(vector<double>(28*28));
      for (int i = 0; i < 28 * 28; i++)
         file >> input.back()[i];
   }
}


int main ()
{
   NeuralNetwork* net;

   char answer;

   cout << "\n~~~~~~~~~~~~~~\n\nload from file? (y/n): ";
   cin >> answer;

   if (answer == 'y')
         net = new NeuralNetwork("mnist_trained.txt");
   else
   {
      vector<unsigned> topology({784, 100, 10});
      vector<string> activation_functions({"sigmoid", "sigmoid", "sigmoid"});
      NeuralNetworkParameters netParams(topology,
                                       activation_functions,
                                       "mse",
                                       true);
            
      net = new NeuralNetwork(netParams);
   }

   Data input, target;
   getMNIST(input, target);
   cout << "\n\ninitial precision: ";
   cout << NeuralTrainer::testClasifier(*net,
                                        input,
                                        target,
                                        int(input.size()*0.9),
                                        -1) << endl;

   int times;
   double learning_rate, inertia;

   cout << "\nnumber of data samples: " << input.size();
   cout << "\n\ninput cycles, learning rate and inertia: ";
   cin >> times >> learning_rate >> inertia;

   NeuralTrainer::basicTraining(*net,
                                input,
                                target,
                                times,
                                learning_rate,
                                inertia,
                                true,
                                true,
                                true,
                                -1,
                                input.size()*0.9);

   cout << "\n\nprecision: ";
   cout << NeuralTrainer::testClasifier(*net,
                                        input,
                                        target,
                                        int(input.size()*0.9),
                                        -1) << endl;

   net->draw();

   std::pair<double, double> info = net->weightInfo();

   std::cout << info.first << " " << info.second << std::endl;

   cout << "\n FINISHED TRAINNING\n\nExport to file? (y/n): ";
   cin >> answer;
   if (answer == 'y')
      net->saveToFile("mnist_trained.txt");
}