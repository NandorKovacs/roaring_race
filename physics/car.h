#ifndef ROARING_RACE_CAR
#define ROARING_RACE_CAR

#include <box2d/b2_body.h>
#include <box2d/b2_friction_joint.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_revolute_joint.h>
#include <box2d/b2_weld_joint.h>
#include <box2d/b2_world.h>

#include <cmath>

// TODO: destructors

const float gravity = 9.81;
const float friciton_coefficient = 0.9; // asphalt, according to top google hit

class Wheel {
 public:
  b2Body* body;

  Wheel(b2World* world, b2Body* ground, b2Vec2 position, bool controlable);

  void set_friction(float mass, float coeff);
  
  void tick();

 private:
  const float width = 0.5, length = 1.0, density = 1.0;

  bool controlable;

  b2FrictionJoint* friction_joint;

  void lateral_velocity_tick();
  void angular_velocity_tick();
  void control_tick();
};

enum WheelID { FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT };

class Car {
 public:
  Car(b2World* world, b2Body* ground, b2Vec2 position);

  void tick() {
    for (Wheel* w : wheel) {
      w->tick();
    }
  }

 private:
  const float density = 1.0, steering_angle = b2_pi * 5 / 32;

  b2Body* hull;

  // front left, front right,
  Wheel* wheel[4];
  b2RevoluteJoint* front_joint[2];
  b2WeldJoint* rear_joint[2];

  void setup_hull();
};

#endif