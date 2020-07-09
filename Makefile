OPTIONS = -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11
ALL_O = main.o NeuralNetwork.o Neuron.o Sigmoid.o MSE.o Tanh.o NeuralTrainer.o

all: main.exe

main.exe: main.o NeuralNetwork.o NeuralTrainer.o
	g++ $(OPTIONS) -o net $(ALL_O)

main.o: main.cc NeuralNetwork.hh NeuralTrainer.hh
	g++ $(OPTIONS) -c main.cc

NeuralNetwork.o: NeuralNetwork.hh NeuralNetwork.cc Neuron.o MSE.o
	g++ $(OPTIONS) -c NeuralNetwork.cc

Neuron.o: Neuron.hh Neuron.cc Sigmoid.o Tanh.o
	g++ $(OPTIONS) -c Neuron.cc

NeuralTrainer.o: NeuralTrainer.cc NeuralTrainer.hh NeuralNetwork.o
	g++ $(OPTIONS) -c NeuralTrainer.cc

MSE.o: MSE.hh MSE.cc CostFunction.hh
	g++ $(OPTIONS) -c MSE.cc

Sigmoid.o: Sigmoid.cc Sigmoid.hh ActivationFunction.hh
	g++ $(OPTIONS) -c Sigmoid.cc

Tanh.o: Tanh.cc Tanh.hh ActivationFunction.hh
	g++ $(OPTIONS) -c Tanh.cc

clean:
	rm *.o

clean_all:
	rm *.o net
