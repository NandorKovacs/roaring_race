#include "window.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

GameWindow::GameWindow() : window(sf::VideoMode(1600, 1200), "roaring race") {
  // TODO: why do we have this variable??
  // origin = {400, 300};
}

void GameWindow::tick() {
  // TODO: why do we have this variable??
  // origin = window.getSize();
  // origin.x /= 2;
  // origin.y /= 2;

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
  state = {{0, 0}, 0, 0};

  sf::Vector2f v[15] = {{0, 0},       {-1.5, -1.5}, {1.5, -1.5}, {1.5, -1.5},
                        {-1.5, -1.5}, {-1.25, -5},  {1.5, -1.5}, {-1.25, -5},
                        {1.25, -5},   {1.25, -5},   {-1.25, -5}, {-2, -7.5},
                        {1.25, -5},   {-2, -7.5},   {2, -7.5}};

  wheel_joint[0] = {-1.5, -1.5};
  wheel_joint[1] = {1.5, -1.5};
  wheel_joint[2] = {-2, -7.5};
  wheel_joint[3] = {2, -7.5};
  wheel_size.x = 0.5;
  wheel_size.y = 1;

  for (sf::Vector2f vv : v) {
    shape.push_back(sf::Vertex(vv, sf::Color::Green));
  }
}

void DrawableCar::draw_wheels(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  for (int i = 0; i < 4; ++i) {
    sf::RectangleShape wheel({wheel_size.x, wheel_size.y});
    wheel.setOrigin({wheel_size.x / 2, wheel_size.y / 2});

    if (i < 2) {
      wheel.setRotation(state.wheel_angle);
    }

    wheel.setPosition(wheel_joint[i]);

    target.draw(wheel, states);
  }
}

void DrawableCar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  // sf::Vector2u origin = target.getSize();
  // origin.x /= 2;
  // origin.y /= 2;

  states.transform.rotate(state.angle + 180, {0, 0});
  states.transform.translate(state.position);

  draw_wheels(target, states);
  target.draw(shape.data(), shape.size(), sf::Triangles, states);
}

void DrawableCar::set_state(CarState new_state) { state = new_state; }
CarState DrawableCar::get_state() { return state; };