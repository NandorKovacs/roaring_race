#ifndef ROARING_RACE_KEYHANDLER
#define ROARING_RACE_KEYHANDLER
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include <unordered_set>

enum Key { FRONT, BACK, LEFT, RIGHT };

// we do the templating in preparation to having analog support.

template <typename T>
class KeyResult {
 public:
  KeyResult(T result) : result{result} {}
  KeyResult() {
    result = T();
  }

  T get() { return result; }

 private:
  T result;
};

using KeyboardResult = KeyResult<bool>;

class InputHandler {
 public:
  InputHandler() {
    mapping[FRONT] = sf::Keyboard::W;
    mapping[LEFT] = sf::Keyboard::A;
    mapping[BACK] = sf::Keyboard::S;
    mapping[RIGHT] = sf::Keyboard::D;
  
    physics_keys = {FRONT, BACK, LEFT, RIGHT};
    gui_keys = {};
  }

  void init_physics_keys(std::unordered_map<Key, KeyboardResult> &result) {
    for (Key k : physics_keys) {
      result[k] = KeyboardResult{pressed(k)};
    }
  }

  void init_gui_keys(std::unordered_map<Key, KeyboardResult> &result) {
    for (Key k : gui_keys) {
      result[k] = KeyboardResult{pressed(k)};
    }
  }

  // this will get more complicated later on, if we implement analog support.
  // for now this will do
  void tick(std::unordered_map<Key, KeyboardResult> &result) {
    for (auto [key, sfml_key] : mapping) {
      result[key] = KeyboardResult{pressed(key)};
    }
  };

 private:
  std::unordered_map<Key, sf::Keyboard::Key> mapping;
  std::unordered_set<Key> gui_keys;
  std::unordered_set<Key> physics_keys;

  bool pressed(Key key) { return sf::Keyboard::isKeyPressed(mapping[key]); }
};

#endif