#include "graphics/window.h"
#include "common/common_data.h"
namespace gui {
int main() {

  GameWindow window{};

  DrawableCar car{};
  CarData state = car.get_state();
  state.wheel_angle[0] = 20;
  state.wheel_angle[1] = 20;
  car.set_state(state);

  window.add_drawable(&car);

  while (window.isOpen()) {
    window.tick();
  }

  return 0;
}
}  // namespace gui

int main() {
  return gui::main();
}