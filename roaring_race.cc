#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#include "graphics/window.h"
#include "physics/car.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

template <typename T>
class MutexVar {
 public:
  MutexVar(T data) : data{data} {}
  MutexVar() : data{} {}

  void set(T new_data) {
    mut.lock();
    data = new_data;
    mut.unlock();
  }

  T get() {
    mut.lock();
    T new_data = data;
    mut.unlock();
    return new_data;
  }

  template <typename U>
  U exec_with(std::function<U(T)> *func) {
    mut.lock();
    U res = *func(data);
    mut.unlock();
    return res;
  }

 private:
  T data;
  std::mutex mut;
};

void physics_main(MutexVar<CarState> &car_state, MutexVar<bool> &is_running) {
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

  TimePoint prev, cur;
  while (true) {
    std::chrono::time_point prev = std::chrono::high_resolution_clock::now();
    if (!is_running.get()) {
      break;
    }

    // setting car state
    car_state.set(
        {sf::Vector2f(car.pos().x, car.pos().y),
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
    
    car.tick();
    world->Step(time_step, velocity_iterations, position_iterations);
  }
}

void window_main(MutexVar<CarState> &car_state, MutexVar<bool> &is_running) {
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

int main() {
  MutexVar<CarState> car_state;
  MutexVar<bool> is_running(true);

  std::thread window_thread(window_main, std::ref(car_state),
                            std::ref(is_running));
  std::thread physics_thread(physics_main, std::ref(car_state),
                             std::ref(is_running));
  window_thread.join();
  physics_thread.join();
}