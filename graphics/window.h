#ifndef ROARING_RACE_WINDOW
#define ROARING_RACE_WINDOW

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <unordered_set>

class GameWindow {
 public:
  GameWindow();
  void tick();

  void add_drawable(sf::Drawable* drawable) {
    drawables.insert(drawable);
  }

  void pop_drawable(sf::Drawable* drawable) {
    drawables.erase(drawable);
  }

  bool isOpen();

 private:
  sf::Vector2u origin;
  sf::RenderWindow window;

  std::unordered_set<sf::Drawable*> drawables;
};

struct CarState {
  sf::Vector2f position;
  float angle;
  float wheel_angle;
};

class DrawableCar : public sf::Drawable {
 public:
  DrawableCar();

  void set_state(CarState new_state);

 private:
  CarState state;

  std::vector<sf::Vertex> shape;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void draw_wheels(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif