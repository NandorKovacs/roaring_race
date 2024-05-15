#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "common/common_data.h"
#include "common/key_handler.h"
#include "graphics/window.h"
#include "physics/car.h"

class MutexInputHandler : public MutexVar<InputHandler> {
 public:
  void init_physics_keys(std::unordered_map<Key, KeyboardResult> &result) {
    mut.lock();
    data.init_physics_keys(result);
    mut.unlock();
  }
  void init_gui_keys(std::unordered_map<Key, KeyboardResult> &result) {
    mut.lock();
    data.init_gui_keys(result);
    mut.unlock();
  }
  void tick(std::unordered_map<Key, KeyboardResult> &result) {
    mut.lock();
    data.tick(result);
    mut.unlock();
  }
};

namespace ph {

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

void physics_main(MutexVar<CarData> &car_state,
                  MutexInputHandler &input_handler,
                  MutexVar<bool> &is_running) {
  b2World *world = new b2World({0, 0});
  std::cout << "starting physics thread" << std::endl;
  // create ground
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position = {0, 0};
  b2Body *ground = world->CreateBody(&body_def);
  // --

  Car car(world, ground, {0, 0});

  float time_step = 1.0f / 60.f;
  int32 velocity_iterations = 10;
  int32 position_iterations = 8;

  std::unordered_map<Key, KeyboardResult> keyboard_input;

  input_handler.init_physics_keys(keyboard_input);
  TimePoint prev, cur;
  while (true) {
    std::chrono::time_point prev = std::chrono::high_resolution_clock::now();
    if (!is_running.get()) {
      break;
    }

    // setting car state
    car_state.set(
        {{car.pos().x, car.pos().y},
         car.angle(),
         {car.wheel_angle(FRONT_LEFT), car.wheel_angle(FRONT_RIGHT)}});
    // --

    // scheduling
    cur = Clock::now();
    std::this_thread::sleep_for(
        std::chrono::duration<double, std::micro>(100000 / 6) -
        std::chrono::duration<double, std::micro>(cur - prev));
    prev = Clock::now();
    // --

    input_handler.tick(keyboard_input);
    std::unordered_set<Action> actions;
    for (auto [key, res] : keyboard_input) {
      if (!res.get()) {
        continue;
      }
      switch (key) {
        case FRONT:
          actions.insert(ACCEL);
          break;
        case BACK:
          actions.insert(BREAK);
          break;
        case Key::LEFT:
          actions.insert(Action::LEFT);
          break;
        case Key::RIGHT:
          actions.insert(Action::RIGHT);
          break;
      }
    }

    car.tick(actions);
    world->Step(time_step, velocity_iterations, position_iterations);
  }
}

}  // namespace ph

namespace gui {

void window_main(MutexVar<CarData> &car_state, MutexInputHandler &input_handler,
                 MutexVar<bool> &is_running) {
  std::cout << "starting window thread" << std::endl;
  GameWindow window{};
  DrawableCar car{};

  window.add_drawable(&car);

  while (window.isOpen()) {
    if (!is_running.get()) {
      window.close();
    }

    car.set_state(car_state.get());

    window.tick();
  }

  is_running.set(false);
}

}  // namespace gui

int main() {
  MutexVar<CarData> car_state;
  MutexVar<bool> is_running(true);
  MutexInputHandler input_handler;

  std::thread window_thread(gui::window_main, std::ref(car_state),
                            std::ref(input_handler), std::ref(is_running));
  std::thread physics_thread(ph::physics_main, std::ref(car_state),
                             std::ref(input_handler), std::ref(is_running));
  window_thread.join();
  physics_thread.join();
}