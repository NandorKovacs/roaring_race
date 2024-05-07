#include "graphics/window.h"

int main() {
  GameWindow window{};
  
  DrawableCar car{};
  CarState state = car.get_state();
  state.wheel_angle[0] = 20;
  state.wheel_angle[1] = 20;
  car.set_state(state);

  window.add_drawable(&car);

  while (window.isOpen()) {
    window.tick();
  }
}