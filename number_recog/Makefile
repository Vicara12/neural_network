OPTIONS = -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11

NECESARY = ../Linear.o ../MSE.o ../NeuralNetwork.o ../Neuron.o ../ReLu.o\
../Sigmoid.o ../Tanh.o

ALL_O = main.o Graphics.o Processing.o

SFML_COMP = -lsfml-system -lsfml-window -lsfml-graphics

all: NumberRecognizer

NumberRecognizer: main.o Graphics.o Processing.o $(NECESARY)
	g++ $(OPTIONS) -o NumberRecognizer $(ALL_O) $(NECESARY) $(SFML_COMP)

main.o: main.cc Processing.hh
	g++ $(OPTIONS) -c main.cc

Graphics.o: Graphics.hh Graphics.cc
	g++ $(OPTIONS) -c Graphics.cc

Processing.o: Processing.hh Processing.cc Graphics.hh
	g++ $(OPTIONS) -c Processing.cc

clean:
	rm $(ALL_O)

clean_all:
	rm $(ALL_o) NumberRecognizer