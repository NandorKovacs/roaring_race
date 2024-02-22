#include "graphics/window.h"

int main() {
  GameWindow window{};
  
  DrawableCar car{};

  window.add_drawable(&car);

  while (window.isOpen()) {
    window.tick();
  }
}