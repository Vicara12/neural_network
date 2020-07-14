#include "NeuralNetwork.hh"

NeuralNetworkParameters::NeuralNetworkParameters(
                           std::vector<unsigned int> nn_topology,
                           std::vector<std::string> nn_activation_functions,
                           std::string nn_cost_function,
                           bool nn_normalize_output)
                           : topology(nn_topology),
                             activation_functions(nn_activation_functions),
                             cost_function(nn_cost_function),
                             normalize_output(nn_normalize_output),
                             weight_max(0.05),
                             bias_initial_value(1),
                             batchs_for_avg_error(100) {}

NeuralNetwork::NeuralNetwork (const NeuralNetworkParameters& netData) :
         total_error(0),
         batchs_for_avg_error(netData.batchs_for_avg_error),
         normalize_output(netData.normalize_output)
{
   if (netData.topology.size() < 2)
      throw InvalidTopology();
   
   if (netData.topology.size() != netData.activation_functions.size())
      throw InvalidActFuncVector();

   for (auto layer_size : netData.topology)
      if (layer_size < 1)
         throw InvalidTopology();
   
   if (netData.batchs_for_avg_error < 1)
      throw InvalidBatchErrorAVGQty();

   layer = std::vector<Layer>(netData.topology.size());
   cost_function = initCostFunction(netData.cost_function);

   srand(time(NULL));

   for (int i = 0; i < layer.size(); i++)
   {
      int inputs = (i == 0 ? 0 : netData.topology[i-1] + 1);
      
      layer[i] = std::vector<Neuron>(netData.topology[i] + 1);

      for (int neuron = 0; neuron < netData.topology[i] + 1; neuron++)
      {
         if (neuron == netData.topology[i])
            layer[i][neuron] = Neuron(netData.bias_initial_value, neuron);
         else
         {
            layer[i][neuron] = Neuron(netData.activation_functions[i],
                                      inputs,
                                      netData.weight_max,
                                      neuron);
         }
      }
   }
}

NeuralNetwork::NeuralNetwork (std::string filePath)
{
   std::fstream file;
   file.open(filePath, std::ios::in);

   if (not file)
      throw ErrorOpeningNeuralNetwork();
   
   if (not (file >> normalize_output))
      throw ErrorLoadingNeuralNetwork();

   if (not (file >> batchs_for_avg_error))
      throw ErrorLoadingNeuralNetwork();
   
   std::string cost_function_name;

   if (not (file >> cost_function_name))
      throw ErrorLoadingNeuralNetwork();
   
   cost_function = initCostFunction(cost_function_name);
   
   int topology_size;

   if (not (file >> topology_size))
      throw ErrorLoadingNeuralNetwork();

   layer = std::vector<Layer>(topology_size);

   // reading and shaping each layers' size
   for (int l = 0; l < topology_size; l++)
   {
      int layer_size;
      if (not (file >> layer_size))
         throw ErrorLoadingNeuralNetwork();

      layer[l] = Layer(layer_size + 1);
   }
   
   std::vector<std::string> activation_function_names(topology_size);

   // activation functions are stored now in order to create the neurons later
   for (int l = 0; l < topology_size; l++)
      if (not (file >> activation_function_names[l]))
         throw ErrorLoadingNeuralNetwork();
   
   // create input layer and read input layer's bias
   double input_layer_bias;
   if (not (file >> input_layer_bias))
      throw ErrorLoadingNeuralNetwork();
   for (int n = 0; n < layer[0].size(); n++)
   {
      if (n == layer[0].size() - 1)
         layer[0][n] = Neuron(input_layer_bias, n);
      else
         layer[0][n] = Neuron(activation_function_names[0], 0, 1, n);
   }

   // now read and create each neuron
   for (int l = 1; l < layer.size(); l++)
   {
      for (int n = 0; n < layer[l].size(); n++)
      {
         if (n == layer[l].size() - 1)
         {
            double layer_bias;
            if (not (file >> layer_bias))
               throw ErrorLoadingNeuralNetwork();
            layer[l].back() = Neuron(layer_bias, n);
         }
         else
         {
            std::vector<double> neuron_w(layer[l-1].size());
            for (int w = 0; w < neuron_w.size(); w++)
               if (not (file >> neuron_w[w]))
                  throw ErrorLoadingNeuralNetwork();
            layer[l][n] = Neuron(activation_function_names[l], neuron_w, n);
         }
      }
   }

   file.close();
}

void NeuralNetwork::saveToFile (std::string filePath) const
{
   std::fstream file;
   file.open(filePath, std::ios::out | std::ios::trunc);

   if (not file)
      throw ErrorSavingNeuralNetwork();
   
   file << normalize_output << std::endl;
   file << batchs_for_avg_error << std::endl;
   file << cost_function->getName() << std::endl;
   
   // write topology
   file << layer.size() << std::endl;
   for (int i = 0; i < layer.size(); i++)
      i == 0 ? file << (layer[i].size() - 1) :
               file << " " << (layer[i].size() - 1);
   file << std::endl;

   // write activation functions
   for (int i = 0; i < layer.size(); i++)
      i == 0 ? file << layer[i][0].activationFunctionName() :
               file << " " << layer[i][0].activationFunctionName();
   file << std::endl;
   
   // write all the weights of the neurons
   file << layer.front().back().getOutput() << std::endl;
   for (int l = 1; l < layer.size(); l++)
   {
      for (int n = 0; n < layer[l].size() - 1; n++)
      {
         std::vector<double> weights = layer[l][n].getWeights();

         for (int w = 0; w < weights.size(); w++)
            w == 0 ? file << weights[w] : file << " " << weights[w];
         file << std::endl;
      }
      file << layer[l].back().getOutput() << std::endl;
   }

   file.close();
}

void NeuralNetwork::compute (const std::vector<double>& input)
{
   if (input.size() != qtyInputs())
      throw InvalidInputSize();
   
   for (int i = 0; i < qtyInputs(); i++)
      layer[0][i].setOutput(input[i]);
   
   for (int l = 1; l < layer.size(); l++)
   {
      for (int n = 0; n < layer[l].size() - 1; n++)
         layer[l][n].calculateOutput(layer[l-1]);
   }
}

void NeuralNetwork::getOutput (std::vector<double>& result)
{
   if (result.size() != qtyOutputs())
      throw InvalidOutputSize();
   
   if (normalize_output)
   {
      double output_sum = 0;

      for (auto neuron : layer.back())
         output_sum += neuron.getOutput();
      
      output_sum -= layer.back().back().getOutput();
      
      double default_output = 1.0/(layer.back().size() - 1);
      
      for (int i = 0; i < qtyOutputs(); i++)
         result[i] = output_sum != 0 ? layer.back()[i].getOutput()/output_sum :
                                       default_output;
   }
   else
      for (int i = 0; i < qtyOutputs(); i++)
         result[i] = layer.back()[i].getOutput();
}

double NeuralNetwork::error (const std::vector<double>& input,
                             const std::vector<double>& target)
{
   this->compute(input);
   double this_error = cost_function->error(layer.back(), target);
   computeAvgError(this_error);
   return this_error;
}

double NeuralNetwork::avgError (const std::vector<double>& input,
                                const std::vector<double>& target)
{
   double this_error = error(input, target);
   return computeAvgError(this_error);   
}

double NeuralNetwork::computeAvgError (double this_error)
{
   total_error += this_error;
   error_list.push_back(this_error);

   if (batchs_for_avg_error < error_list.size())
   {
      total_error -= error_list.front();
      error_list.pop_front();
   }

   return total_error / error_list.size();
}

void NeuralNetwork::clearAvgError ()
{
   total_error = 0;
   error_list.clear();
}

void NeuralNetwork::changeBatchsForAVGError (unsigned qty)
{
   if (qty < 1)
      throw InvalidBatchErrorAVGQty();
   
   batchs_for_avg_error = qty;

   while (error_list.size() > batchs_for_avg_error)
   {
      total_error -= error_list.front();
      error_list.pop_front();
   }
}

double NeuralNetwork::backPropagate (const std::vector<double>& input,
                                     const std::vector<double>& target,
                                     double learning_rate,
                                     double inertia,
                                     bool train_bias)
{
   this->compute(input);

   cost_function->dError(layer.back(), target);

   double avg_err = computeAvgError(cost_function->error(layer.back(), target));

   // calculate the gradients in the entire network
   for (int l = layer.size() - 2; l >= 1; l--)
      for (int n = 0; n < layer[l].size() - not train_bias; n++)
         layer[l][n].calculateGradient(layer[l+1]);

   // calc the gradient of the bias at the input layer
   if (train_bias)
      layer.front()[layer.front().size()-1].calculateGradient(layer[1]);

   // update weights in the entire network
   for (int l = layer.size() - 1; l >= 1; l--)
      for (int n = 0; n < layer[l].size() - not train_bias; n++)
         layer[l][n].actualize(layer[l-1], learning_rate, inertia);
   
   // calc the gradient of the bias at the input layer
   if (train_bias)
      layer.front()[layer.front().size()-1].actualize(layer[0],
                                                      learning_rate,
                                                      inertia);
   
   return avg_err;
}

void NeuralNetwork::draw () const
{
   std::cout << "topology:";
   for (auto l : layer)
      std::cout << " " << (l.size() - 1);
   std::cout << "\nactivation functions:";
   for (auto l : layer)
      std::cout << " " << l[0].activationFunctionName();
   std::cout << "\ncost function: " << cost_function->getName();
   std::cout << "\nnormalized output: ";
   normalize_output ? std::cout << "yes" : std::cout << "no";
   std::cout << "\nweights:";

   for (int l = 0; l < layer.size(); l++)
   {
      std::cout << "\n\n  *layer: " << l << std::endl;

      if (l == 0)
      {
         for (int n = 0; n < layer[l].size() - 1; n++)
            std::cout << "   input neuron\n";
         std::cout << "   ";
         layer.front().back().draw();
      }
      else
      {
         for (int n = 0; n < layer[l].size() - (l == layer.size() - 1); n++)
         {
            std::cout << "   ";
            layer[l][n].draw();
         }
      }
   }

   std::cout << std::endl << std::endl;
}

std::pair<double, double> NeuralNetwork::weightInfo () const
{
   double sum = 0, qty = 0, max = -1;

   for (auto l : layer)
      for (auto n : l)
      {
         std::vector<double> weights = n.getWeights();

         for (auto w : weights)
         {
            double weight = w < 0 ? -w : w;
            sum += weight;
            ++qty;

            if (weight > max)
               max = weight;
         }
      }
   
   return std::make_pair(sum/qty, max);
}

unsigned int NeuralNetwork::qtyInputs () const
{
   return layer[0].size() - 1;
}

unsigned int NeuralNetwork::qtyOutputs () const
{
   return layer.back().size() - 1;
}

CostFunction* NeuralNetwork::initCostFunction (std::string cost_function_name)
{
   if (cost_function_name == "mse") return new MSE(normalize_output);
   else throw CostFunctionNotFound();

   return NULL;
}