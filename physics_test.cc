#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include <iostream>

#include "physics/car.h"

// mostly tests if the program compiles

b2Body* create_ground(b2World* world) {
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position = {0, 0};
  return world->CreateBody(&body_def);
}

int main() {
  b2World* world = new b2World({0, 0});
  b2Body* ground = create_ground(world);

  Car car(world, ground, {0, 0});

  float time_step = 1.0f / 60.f;
  int32 velocity_iterations = 10;
  int32 position_iterations = 8;

  while (true) {
    std::cout << "car: " << "{" << car.pos().x << ", " << car.pos().y << "}" << std::endl;
    car.tick();
    world->Step(time_step, velocity_iterations, position_iterations);
  }
}