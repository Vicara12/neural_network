#ifndef PROCESSING_HH_
#define PROCESSING_HH_


#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include "Graphics.hh"
#include "../NeuralNetwork.hh"

// processing definitions namespace
namespace pdf
{
   const unsigned int BRUSH_RADIOUS = 30;
   const unsigned int BRUSH_MAX_VAL = 255;
   const unsigned int BRUSH_DISTANCE = 0;

   const std::string NET_PATH = "mnist_trained.txt";
}


class Processing
{
public:
   Processing ();

private:
   void mainLoop ();
   bool buttonPressed (int i);
   void processBrush ();
   double blockPointDist(int bx, int by, int px, int py);
   double getCubeFade (double proportion) const;

   Graphics graphics;
   sf::RenderWindow window;
   Screen screen;
   std::vector<double> output;
   bool painting;
   unsigned min_mouse_distance;  // distance the mouse must move to paint
   unsigned last_mouse_x;        // last x position of the mouse when painted
   unsigned last_mouse_y;        // last y position of the mouse when painted
   NeuralNetwork neural_network;
};


#endif