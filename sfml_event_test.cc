#include <SFML/Window.hpp>
#include <iostream>

int main() {
  sf::Window window(sf::VideoMode(800, 600), "My window");

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::KeyPressed) {
        std::cout << "{" << std::endl;
        std::cout << "  alt: " << event.key.alt << std::endl;
        std::cout << "  code: " << event.key.code << std::endl;
        std::cout << "  control: " << event.key.control << std::endl;
        std::cout << "  scancode: " << event.key.scancode << std::endl;
        std::cout << "  shift: " << event.key.shift << std::endl;
        std::cout << "  system: " << event.key.system << std::endl;
        std::cout << "}" << std::endl;
      }
    }
  }

  return 0;
}