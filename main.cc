#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "physics/car.h"

b2Body* create_ground(b2World* world) {
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position = {0, 0};
  return world->CreateBody(&body_def);
}

int main() {
  b2World* world = new b2World({0, 0});
  b2Body* body = create_ground(world);
}