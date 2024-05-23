#ifndef ROARING_RACE_RAYLIB_WINDOW
#define ROARING_RACE_RAYLIB_WINDOW

#include <raylib.h>

#include <array>
#include <unordered_set>
#include <vector>

#include "common/common_data.h"

namespace gui {

class Drawable {
 public:
  virtual void draw() = 0;
};

class GameWindow {
 public:
  GameWindow();
  void tick();

  void add_drawable(Drawable* drawable) { drawables.insert(drawable); };
  void pop_drawable(Drawable* drawable) { drawables.erase(drawable); };

  bool is_open();
  void close();

 private:
  Camera2D camera;

  const double TARGET_PPMM = 3.77358490566;
  double default_zoom = 1.0;

  std::unordered_set<Drawable*> drawables;
};

class DrawableCar : public Drawable {
 public:
  DrawableCar();

  void set_state(CarData new_state) { state = new_state; };
  CarData get_state() { return state; };

  void draw();

 private:
  CarData state;

  std::vector<Vector2> triangles;
  Vector2 wheel_size;
  std::array<Vector2, 4> wheel_joint;
};

}  // namespace gui

#endif