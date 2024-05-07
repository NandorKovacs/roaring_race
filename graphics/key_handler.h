#ifndef ROARING_RACE_KEYHANDLER
#define ROARING_RACE_KEYHANDLER
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

enum Key {
  FRONT, BACK, LEFT, RIGHT
};


class InputHandler {
 public:
  InputHandler() {
    mapping[FRONT] = sf::Keyboard::W;
    mapping[LEFT] = sf::Keyboard::A;
    mapping[BACK] = sf::Keyboard::S;
    mapping[RIGHT] = sf::Keyboard::D;
  }

  bool pressed(Key key) {
    return sf::Keyboard::isKeyPressed(mapping[key]);
  }

 private:
  std::unordered_map<Key, sf::Keyboard::Key> mapping;
};

#endif