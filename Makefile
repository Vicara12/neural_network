OPTIONS = -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11

ALL_O = main.o NeuralNetwork.o Neuron.o Sigmoid.o MSE.o Tanh.o NeuralTrainer.o\
Linear.o ReLu.o

ALL = ActivationFunction.hh CostFunction.hh main.cc Makefile MSE.cc MSE.hh\
NeuralNetwork.cc NeuralNetwork.hh NeuralTrainer.cc NeuralTrainer.hh Neuron.cc\
Neuron.hh Sigmoid.cc Sigmoid.hh Tanh.cc Tanh.hh plotter.py Linear.hh Linear.cc\
ReLu.hh ReLu.cc

NUMBER_RECOG = ./number_recog/*.cc ./number_recog/*.hh ./number_recog/Makefile

all: main.exe

main.exe: main.o NeuralNetwork.o NeuralTrainer.o
	g++ $(OPTIONS) -o net $(ALL_O)

main.o: main.cc NeuralNetwork.hh NeuralTrainer.hh
	g++ $(OPTIONS) -c main.cc

NeuralNetwork.o: NeuralNetwork.hh NeuralNetwork.cc Neuron.o MSE.o
	g++ $(OPTIONS) -c NeuralNetwork.cc

Neuron.o: Neuron.hh Neuron.cc Sigmoid.o Tanh.o Linear.o ReLu.o
	g++ $(OPTIONS) -c Neuron.cc

NeuralTrainer.o: NeuralTrainer.cc NeuralTrainer.hh NeuralNetwork.o
	g++ $(OPTIONS) -c NeuralTrainer.cc

MSE.o: MSE.hh MSE.cc CostFunction.hh
	g++ $(OPTIONS) -c MSE.cc

Sigmoid.o: Sigmoid.cc Sigmoid.hh ActivationFunction.hh
	g++ $(OPTIONS) -c Sigmoid.cc

Tanh.o: Tanh.cc Tanh.hh ActivationFunction.hh
	g++ $(OPTIONS) -c Tanh.cc

Linear.o: Linear.hh Linear.cc ActivationFunction.hh
	g++ $(OPTIONS) -c Linear.cc

ReLu.o: ReLu.hh ReLu.cc ActivationFunction.hh
	g++ $(OPTIONS) -c ReLu.cc

clean:
	rm *.o

clean_all:
	rm *.o net

git:
	git add $(ALL) $(NUMBER_RECOG)

lines:
	wc $(ALL) | tail -1