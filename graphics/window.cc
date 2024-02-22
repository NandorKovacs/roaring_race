#include "window.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

GameWindow::GameWindow() : window(sf::VideoMode(1600, 1200), "roaring race") {
  origin = {400, 300};
}

void GameWindow::tick() {
  origin = window.getSize();
  origin.x /= 2;
  origin.y /= 2;

  sf::Event event;
  while (window.pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) window.close();
  }
  window.clear(sf::Color::Black);

  for (sf::Drawable* d : drawables) {
    window.draw(*d);
  }

  sf::View view{
      {0, 0}, (sf::Vector2f){window.getSize().x / 50, window.getSize().y / 50}};
  // view.rotate(180);
  window.setView(view);

  window.display();
}

bool GameWindow::isOpen() { return window.isOpen(); }

DrawableCar::DrawableCar() {
  sf::Vector2f v[15] = {{0, 0},       {-1.5, -1.5}, {1.5, -1.5}, {1.5, -1.5},
                        {-1.5, -1.5}, {-1.25, -5},  {1.5, -1.5}, {-1.25, -5},
                        {1.25, -5},   {1.25, -5},   {-1.25, -5}, {-2, -7.5},
                        {1.25, -5},   {-2, -7.5},   {2, -7.5}};

  state = {{0, 0}, 0, 0};

  for (sf::Vector2f vv : v) {
    shape.push_back(sf::Vertex(vv, sf::Color::Green));
  }
}

void DrawableCar::draw_wheels(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  sf::Vector2f wheel_joint[4] = {
      {-1.5, -1.5}, {1.5, -1.5}, {-2, -7.5}, {2, -7.5}};

  for (int i = 0; i < 4; ++i) {
    sf::RectangleShape wheel({0.5, 1});
    wheel.setOrigin({0.25, 0.5});

    if (i < 2) {
      wheel.setRotation(state.wheel_angle);
    }

    wheel.setPosition(wheel_joint[i]);

    target.draw(wheel, states);
  }
}

void DrawableCar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  sf::Vector2u origin = target.getSize();
  origin.x /= 2;
  origin.y /= 2;

  states.transform.rotate(state.angle + 180, {0, 0});
  states.transform.translate(state.position);

  draw_wheels(target, states);
  target.draw(shape.data(), shape.size(), sf::Triangles, states);
}

void DrawableCar::set_state(CarState new_state) { state = new_state; }