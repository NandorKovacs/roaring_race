#include "graphics/window.h"

int main() {
  GameWindow window{};
  
  DrawableCar car{};

  window.add_drawable(&car);

  while (true) {
    window.tick();
  }
}