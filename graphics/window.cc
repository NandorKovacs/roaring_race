#include "window.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>

GameWindow::GameWindow() : window(sf::VideoMode(800, 600), "roaring race") {
  origin = {400, 300};
}

void GameWindow::tick() {
  origin = window.getSize();
  origin.x /= 2;
  origin.y /= 2;

  window.clear(sf::Color::White);

  sf::Event event;
  while (window.pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) window.close();
  }

  for (sf::Drawable* d : drawables) {
    window.draw(*d);
  }

  sf::CircleShape cs = sf::CircleShape(50, 100);
  cs.setFillColor(sf::Color::Red);
  cs.setOrigin({400, 200});
  window.draw(cs);

  window.display();
}

DrawableCar::DrawableCar() {
  sf::Vector2f v[11] =
  { {0, 0},
    {-1.5, -1.5},
    {1.5, -1.5},
    {1.5, -1.5},
    {-1.5, -1.5},
    {-1.25, -5},
    {1.25, -5},
    {1.25, -5},
    {-1.25, -5},
    {-2, -7.5},
    {2, -7.5} };

  state = {{0,0},0,0};

  shape.setPrimitiveType(sf::TriangleStrip);
  for (sf::Vector2f vv : v) {    
    shape.append(sf::Vertex(vv, sf::Color::Green));
  }
}

void DrawableCar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  sf::Vector2u origin = target.getSize();
  origin.x /= 2;
  origin.y /= 2;

  states.transform.rotate(state.angle, {0,0});
  states.transform.scale(100, 100);
  states.transform.translate(state.position);

  target.draw(shape, states);
}

void DrawableCar::set_state(CarState new_state) {
  state = new_state;
}