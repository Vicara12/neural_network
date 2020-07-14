#include "Processing.hh"

#include <iostream>

Processing::Processing () : graphics(window),
                           window(sf::VideoMode(gdf::WINDOW_WIDTH,
                                                 gdf::WINDOW_HEIGHT),
                                   "Number recognizer"),
                            screen(gdf::N_BOXES,
                                   std::vector<unsigned char>(gdf::N_BOXES, 0)),
                            output(gdf::N_BARS, 0),
                            painting(true),
                            min_mouse_distance(pdf::BRUSH_DISTANCE),
                            last_mouse_x(-2*pdf::BRUSH_RADIOUS),
                            last_mouse_y(-2*pdf::BRUSH_RADIOUS),
                            neural_network(pdf::NET_PATH)
{
   //window.setVerticalSyncEnabled(true);
   window.setFramerateLimit(30);
   graphics.displayGraphics(screen, output, painting);

   mainLoop();
}

void Processing::mainLoop ()
{
   while (window.isOpen())
   {
      sf::Event event;
      if (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      {

         if (buttonPressed(0))
            painting = true;
         else if (buttonPressed(1))
            painting = false;
         else if (buttonPressed(2))
            screen = Screen(gdf::N_BOXES,
                            std::vector<unsigned char>(gdf::N_BOXES, 0));
         else
            processBrush();
         
         std::vector<double> screen_double(gdf::N_BOXES*gdf::N_BOXES);
         
         for (int i = 0; i < gdf::N_BOXES; i++)
            for (int j = 0; j < gdf::N_BOXES; j++)
               screen_double[i*gdf::N_BOXES + j] = screen[i][j];
         
         neural_network.compute(screen_double);
         neural_network.getOutput(output);

         graphics.displayGraphics(screen, output, painting);
      }
   }
}

bool Processing::buttonPressed (int i)
{
   int button_horizontal = gdf::BORDER + gdf::N_BOXES * gdf::BOX_SIZE +
                           (gdf::N_BOXES - 1) * gdf::BOX_SEPARATION +
                           gdf::BAR_WIDTH + 2*i*gdf::BUTTON_SIZE +
                           i*gdf::BAR_SEPARATION;
   
   int posx = sf::Mouse::getPosition(window).x;
   int posy = sf::Mouse::getPosition(window).y;
   
   return (posy >= gdf::BORDER and posy < gdf::BORDER + gdf::BUTTON_SIZE) and
          (posx >= button_horizontal and
               posx < button_horizontal + 2*gdf::BUTTON_SIZE);
}

void Processing::processBrush ()
{
   int posx = sf::Mouse::getPosition(window).x;
   int posy = sf::Mouse::getPosition(window).y;

   if (sqrt(pow(last_mouse_x - posx, 2) + pow(last_mouse_y - posy, 2)) <
                                                         min_mouse_distance)
      return;
   
   last_mouse_x = posx;
   last_mouse_y = posy;

   // only actualize a square around the mouse proportional to the brush
   unsigned minx = (posx-gdf::BORDER)/(gdf::BOX_SIZE + gdf::BOX_SEPARATION) + 1;
   unsigned miny = (posy-gdf::BORDER)/(gdf::BOX_SIZE + gdf::BOX_SEPARATION) + 1;
   unsigned block_radious = pdf::BRUSH_RADIOUS/
                            (gdf::BOX_SIZE + gdf::BOX_SEPARATION) + 1;
   unsigned maxx = minx + block_radious, maxy = miny + block_radious;
   minx -= block_radious;
   miny -= block_radious;

   minx = std::max(unsigned(0), minx);
   miny = std::max(unsigned(0), miny);
   maxx = std::min(unsigned(gdf::N_BOXES - 1), maxx);
   maxy = std::min(unsigned(gdf::N_BOXES - 1), maxy);

   // actualize the box
   for (unsigned i = miny; i <= maxy; i++)
      for (unsigned j = minx; j <= maxx; j++)
      {
         unsigned char new_val = pdf::BRUSH_MAX_VAL;
         double proportion = blockPointDist(j, i, posx, posy);

         proportion = getCubeFade(proportion);

         if (painting)
         {
            new_val *= 1 - proportion;
            screen[i][j] = std::max(screen[i][j], new_val);
         }
         else
         {
            new_val *= proportion;
            screen[i][j] = std::min(screen[i][j], new_val);
         }
      }
}

double Processing::blockPointDist(int bx, int by, int px, int py)
{
   double posbx, posby;

   posbx = gdf::BORDER +
           gdf::BOX_SIZE/2 +
           bx*(gdf::BOX_SIZE + gdf::BOX_SEPARATION);

   posby = gdf::UPPER_BORDER +
           gdf::BOX_SIZE/2 +
           by*(gdf::BOX_SIZE + gdf::BOX_SEPARATION);
   
   return sqrt(pow(posbx - px, 2) + pow(posby - py, 2));
}

double Processing::getCubeFade (double proportion) const
{
   double intensity = painting ? 1.5 : 5;
   proportion = pow(proportion, intensity) / pow(pdf::BRUSH_RADIOUS, intensity);

   return proportion > 1.0 ? 1.0 : proportion;
}