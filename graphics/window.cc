#include "window.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
namespace gui {

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
void GameWindow::close() { window.close(); }

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
  wheel_size.x = 0.75;
  wheel_size.y = 1.5;

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
      wheel.setRotation(state.wheel_angle[i]);
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
  states.transform.translate(state.position).rotate(state.angle, {0, 0});

  draw_wheels(target, states);
  target.draw(shape.data(), shape.size(), sf::Triangles, states);
}

void DrawableCar::set_state(CarData new_state) {
  state.angle = new_state.angle;
  state.wheel_angle[0] = new_state.wheel_angle[0];
  state.wheel_angle[1] = new_state.wheel_angle[1];
  state.position =
      sf::Vector2f{new_state.position.first, new_state.position.second};
}
CarData DrawableCar::get_state() {
  CarData res;
  res.position = {state.position.x, state.position.y};
  res.angle = state.angle;
  res.wheel_angle[0] = state.wheel_angle[0];
  res.wheel_angle[1] = state.wheel_angle[1];
  return res;
};

}  // namespace gui