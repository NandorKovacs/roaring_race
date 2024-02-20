#ifndef ROARING_RACE_WINDOW
#define ROARING_RACE_WINDOW

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>


class GameWindow {
 public:
  GameWindow();
  void tick();
 private:
  sf::RenderWindow window;
};

class DrawableCar : public sf::Drawable {
 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif