#include "graphics/window.h"

int main() {
  GameWindow window{};
  
  while (true) {
    window.tick();
  }
}