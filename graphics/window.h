#ifndef ROARING_RACE_WINDOW
#define ROARING_RACE_WINDOW

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <unordered_set>
#include <common/common_data.h>

namespace gui {

class GameWindow {
 public:
  GameWindow();
  void tick();

  void add_drawable(sf::Drawable* drawable) { drawables.insert(drawable); }
  void pop_drawable(sf::Drawable* drawable) { drawables.erase(drawable); }

  bool isOpen();
  void close();

 private:
  // TODO: why do we have this variable??
  // sf::Vector2u origin;
  sf::RenderWindow window;

  std::unordered_set<sf::Drawable*> drawables;
};

struct CarState {
  sf::Vector2f position = {0, 0};
  float angle = 0;
  float wheel_angle[2] = {0, 0};
};

class DrawableCar : public sf::Drawable {
 public:
  DrawableCar();

  void set_state(CarData new_state);
  CarData get_state();

 private:
  CarState state;

  std::vector<sf::Vertex> shape;

  sf::Vector2f wheel_size;
  sf::Vector2f wheel_joint[4];

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void draw_wheels(sf::RenderTarget& target, sf::RenderStates states) const;
};

}  // namespace gui

#endif