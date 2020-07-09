#include "NeuralTrainer.hh"

double NeuralTrainer::basicTraining (NeuralNetwork& net,
                                     Data input,
                                     Data target,
                                     int times,
                                     double learning_rate,
                                     bool show_progress,
                                     bool plot)
{
   if (input.size() != target.size())
      throw InputAndOutputSizesDoNotMAtch();
   
   if (times < 1)
      throw InvalidTimes();

   net.clearAvgError();
   double error = 1;
   int last_progress_shown = 0;
   std::list<double> error_list;

   for (int i = 0; i < times; i++)
   {
      if (show_progress and (i+1)*100/times > last_progress_shown)
      {
         last_progress_shown = (i+1)*100/times;
         std::cout << "progress: " << last_progress_shown << "%";
         if (last_progress_shown < 10) std::cout << " ";
         if (last_progress_shown < 100) std::cout << " ";
         std::cout << " -  error: " << error << std::endl;
      }

      int batch = rand() % input.size();
      error = net.backPropagate(input[batch], target[batch], learning_rate);

      if (plot)
         error_list.push_back(error);
   }

   if (plot)
      plotError(error_list);

   return error;
}

std::pair<double, double> NeuralTrainer::normalTraining (NeuralNetwork& net,
                                                         Data input,
                                                         Data target,
                                                         int times,
                                                         double learning_rate,
                                                         double test_percentage,
                                                         bool show_progress,
                                                         bool plot)
{
   if (times < 1)
      throw InvalidTimes();
   
   if (input.size() != target.size())
      throw InputAndOutputSizesDoNotMAtch();

   int qty_training_samples = input.size() * (100 - test_percentage) / 100;

   if (qty_training_samples == input.size())
      throw TestPercentageIsTooSmall();

   std::pair<double, double> error;
   int last_progress_shown = 0;
   std::list<double> error_list;
   net.clearAvgError();

   for (int i = 0; i < times; i++)
   {
      if (show_progress and (i+1)*100/times > last_progress_shown)
      {
         last_progress_shown = (i+1)*100/times;
         std::cout << "progress: " << last_progress_shown << "%";
         if (last_progress_shown < 10) std::cout << " ";
         if (last_progress_shown < 100) std::cout << " ";
         std::cout << " -  error: " << error.first << std::endl;
      }

      int item = rand() % qty_training_samples;

      error.first = net.backPropagate(input[item], target[item], learning_rate);

      if (plot)
         error_list.push_back(error.first);
   }

   net.clearAvgError();

   double total_error = 0;

   for (int i = qty_training_samples; i < input.size(); i++)
      total_error += net.error(input[i], target[i]);

   error.second = total_error / (input.size() - qty_training_samples);

   if (plot)
      plotError(error_list);

   return std::make_pair(0,0);
}

void NeuralTrainer::plotError (const std::list<double>& error_list)
{
   std::fstream file;
   file.open("error_list_to_plot", std::ios::out | std::ios::trunc);

   if (not file)
      throw UnableToPlotError();

   for (auto err : error_list)
      file << err << std::endl;
   
   file.close();

   int result = fork();

   if (result == 0)
   {
      close(2);
      execlp("./plotter.py", "plotter.py", NULL);
   }
   else if (result < 0)
      throw UnableToPlotError();
}