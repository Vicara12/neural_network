#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <SFML/Graphics.hpp>
#include <vector>

typedef std::vector<std::vector<unsigned char>> Screen;

// graphical variables definition
namespace gdf
{
   const unsigned BUTTON_SIZE =50;
   const unsigned N_BOXES = 28;
   const unsigned BOX_SIZE = 25;
   const unsigned BOX_SEPARATION = 3;
   const unsigned N_BARS = 10;
   const unsigned BAR_WIDTH = 40;
   const unsigned BAR_MAX_HEGIHT = 500;
   const unsigned BAR_SEPARATION = 10;
   const unsigned BORDER = 20;
   const unsigned UPPER_BORDER = 20;
   const unsigned TEXT_SIZE_SMALL = 15;
   const unsigned TEXT_SIZE_BIG = 30;
   const unsigned WINDOW_WIDTH = 2*BORDER + 
                                 N_BOXES*BOX_SIZE +
                                 (N_BOXES - 1)*BOX_SEPARATION +
                                 (N_BARS + 2)*BAR_WIDTH +
                                 (N_BARS - 1)*BAR_SEPARATION;
   const unsigned WINDOW_HEIGHT = BORDER + UPPER_BORDER +
                                  N_BOXES * BOX_SIZE + 
                                  (N_BOXES - 1)*BOX_SEPARATION;

   const sf::Color BAR_COLOR = sf::Color(0,150, 250);
   const sf::Color BKGND_COLOR = sf::Color(200, 230, 230);
   const sf::Color TXT_COLOR = sf::Color::Black;
   const sf::Color BUTTON_COLOR = sf::Color::White;
};


class Graphics
{
public:
   Graphics (sf::RenderWindow& disp_window);
   void displayGraphics (const Screen& screen,
                         std::vector<double> outputs,
                         bool painting);

private:
   void drawSquare (int i, int j, unsigned char val);
   void drawBar (int i, double val);
   void drawButton (int i, std::string button_text);
   unsigned getResult (const std::vector<double>& output);

   std::vector<std::vector<char>> screen;
   sf::RenderWindow& window;
   sf::Font font;
   sf::RectangleShape square;
   sf::RectangleShape bar;
   sf::RectangleShape button;
   sf::Text small_txt;
   sf::Text big_txt;
   sf::Text box_txt;
};


#endif