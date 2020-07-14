#include "Graphics.hh"

Graphics::Graphics (sf::RenderWindow& disp_window) : window(disp_window)
{
   square.setSize(sf::Vector2f(gdf::BOX_SIZE, gdf::BOX_SIZE));
   button.setSize(sf::Vector2f(2*gdf::BUTTON_SIZE, gdf::BUTTON_SIZE));
   button.setFillColor(gdf::BUTTON_COLOR);

   bar.setFillColor(gdf::BAR_COLOR);

   font.loadFromFile("Asleepytiming.otf");
   small_txt.setFont(font);
   big_txt.setFont(font);
   box_txt.setFont(font);
   small_txt.setCharacterSize(gdf::TEXT_SIZE_SMALL);
   box_txt.setCharacterSize(gdf::TEXT_SIZE_SMALL);
   big_txt.setCharacterSize(gdf::TEXT_SIZE_BIG);
   small_txt.setFillColor(gdf::TXT_COLOR);
   big_txt.setFillColor(gdf::TXT_COLOR);
}

void Graphics::displayGraphics (const Screen& screen,
                                std::vector<double> outputs,
                                bool painting)
{
   window.clear(gdf::BKGND_COLOR);

   int counter = 0;

   // draw boxes
   for (int i = 0; i < gdf::N_BOXES; i++)
      for (int j = 0; j < gdf::N_BOXES; j++)
      {
         drawSquare(i, j, screen[i][j]);
         ++counter;
      }
   
   // draw bars
   for (int i = 0; i < gdf::N_BARS; i++)
      drawBar(i, outputs[i]);
   
   // draw predicted number string
   big_txt.setPosition(gdf::BORDER + gdf::N_BOXES * gdf::BOX_SIZE +
                       (gdf::N_BOXES - 1) * gdf::BOX_SEPARATION +
                       gdf::BAR_WIDTH,
                       gdf::WINDOW_HEIGHT - gdf::BORDER -
                       3*gdf::TEXT_SIZE_SMALL - gdf::BAR_MAX_HEGIHT -
                       gdf::TEXT_SIZE_BIG);

   big_txt.setString("NUMBER: "+std::to_string(getResult(outputs)));
   window.draw(big_txt);

   // draw painting mode string
   big_txt.setPosition(gdf::BORDER + gdf::N_BOXES * gdf::BOX_SIZE +
                       (gdf::N_BOXES - 1) * gdf::BOX_SEPARATION +
                       gdf::BAR_WIDTH,
                       gdf::BORDER + gdf::BUTTON_SIZE + gdf::TEXT_SIZE_BIG);
   
   big_txt.setString(painting ? "PAINTING" : "ERASING");
   window.draw(big_txt);

   // draw buttons
   drawButton(0, "PAINT");
   drawButton(1, "ERASE");
   drawButton(2, "CLEAR");

   window.display();
}

void Graphics::drawSquare (int x, int y, unsigned char val)
{
   int pv, ph;    // horizontal and vertical block positions
   ph = gdf::BORDER + y * gdf::BOX_SIZE + y*gdf::BOX_SEPARATION;
   pv = gdf::UPPER_BORDER + x * gdf::BOX_SIZE + x*gdf::BOX_SEPARATION;

   square.setFillColor(sf::Color(val,val,val));
   square.setPosition(ph, pv);

   box_txt.setString(std::to_string(val));
   box_txt.setPosition(ph + gdf::BOX_SIZE/((val >= 100) + 3), pv);
   box_txt.setFillColor(val < 128 ? sf::Color::White : sf::Color::Black);

   window.draw(square);
   window.draw(box_txt);
}

void Graphics::drawBar (int i, double val)
{
   int pv, ph;    // horizontal and vertical block positions
   pv = gdf::WINDOW_HEIGHT - gdf::BORDER -
        gdf::TEXT_SIZE_SMALL - val*gdf::BAR_MAX_HEGIHT;
   ph = gdf::BORDER + gdf::N_BOXES * gdf::BOX_SIZE +
        (gdf::N_BOXES - 1) * gdf::BOX_SEPARATION +
        (i + 1)*gdf::BAR_WIDTH + i*gdf::BAR_SEPARATION;
   
   int val_int = int(val*100);

   bar.setPosition(ph, pv);
   bar.setSize(sf::Vector2f(gdf::BAR_WIDTH, gdf::BAR_MAX_HEGIHT*val));
   small_txt.setString(std::to_string(i) + ": " + std::to_string(val_int));
   small_txt.setPosition(ph,// + gdf::BAR_WIDTH/((val_int == 100) + 2),
                         gdf::WINDOW_HEIGHT -
                         gdf::BORDER - gdf::TEXT_SIZE_SMALL);

   window.draw(small_txt);
   window.draw(bar);
}

unsigned Graphics::getResult (const std::vector<double>& output)
{
   int max = 0;

   for (int i = 1; i < output.size(); i++)
      if (output[i] > output[max])
         max = i;
   
   return max;
}

void Graphics::drawButton (int i, std::string button_text)
{
   int button_horizontal = gdf::BORDER + gdf::N_BOXES * gdf::BOX_SIZE +
                           (gdf::N_BOXES - 1) * gdf::BOX_SEPARATION +
                           gdf::BAR_WIDTH + 2*i*gdf::BUTTON_SIZE +
                           i*gdf::BAR_SEPARATION;

   button.setPosition(button_horizontal, gdf::UPPER_BORDER);
   
   big_txt.setPosition(button_horizontal + gdf::BUTTON_SIZE/3,
                       gdf::BORDER + gdf::TEXT_SIZE_BIG/6);
   big_txt.setString(button_text);

   window.draw(button);
   window.draw(big_txt);
}