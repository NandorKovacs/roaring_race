#include "graphics/raylib_window.h"
#include "common/common_data.h"
namespace gui {
int main() {

  GameWindow window{};
  DrawableCar car{};

  CarData state = car.get_state();
  state.wheel_angle[0] = PI/9;
  state.wheel_angle[1] = PI/9;
  state.angle = PI/6;
  car.set_state(state);

  window.add_drawable(&car);

  while (window.is_open()) {
    window.tick();
  }

  return 0;
}
}  // namespace gui

int main() {
  return gui::main();
}