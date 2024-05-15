#include "graphics/window.h"

namespace gui {
int main() {
  GameWindow window{};

  while (true) {
    window.tick();
  }
}
}  // namespace gui

int main() {
  return gui::main();
}