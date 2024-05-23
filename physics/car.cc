#include "car.h"

#include <iostream>
#include <unordered_set>

namespace ph {
Wheel::Wheel(b2World* world, b2Body* ground, b2Vec2 position, bool controlable)
    : controlable{controlable} {
  // create wheel physics object
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position = position;

  b2PolygonShape shape;
  shape.SetAsBox(width, length);

  body = world->CreateBody(&body_def);
  // currently no need to save fixture, retrive here if needed
  body->CreateFixture(&shape, density);

  // create friction joint to ground object
  b2FrictionJointDef joint_def;
  joint_def.bodyA = ground;
  joint_def.bodyB = body;
  joint_def.localAnchorA.SetZero();
  joint_def.localAnchorB = body->GetLocalCenter();

  friction_joint = (b2FrictionJoint*)world->CreateJoint(&joint_def);
}

void Wheel::set_friction(float mass, float coeff) {
  friction_joint->SetMaxForce(mass * gravity * coeff);

  // unsure if this makes sense
  friction_joint->SetMaxTorque(mass * gravity * coeff *
                               b2Distance({0, 0}, {width / 2, length / 2}));
}

void Wheel::tick(std::unordered_set<Action> const& actions) {
  // TODO: stop angular acceleration if reached treshold -> move joint memory
  // from Car to Wheel
  // TODO: bounce back tires to natural automatically

  int accel = 0;
  int right = 0;

  if (controlable) {
    for (Action action : actions) {
      switch (action) {
        case ACCEL:
          ++accel;
          break;
        case BREAK:
          --accel;
          break;
        case RIGHT:
          ++right;
          break;
        case LEFT:
          --right;
          break;
      }
    }
  }

  angular_velocity_tick(right);
  lateral_velocity_tick(accel);
}

void Wheel::lateral_velocity_tick(int accelerate) {
  b2Vec2 lateral_direction = body->GetWorldVector({1, 0});
  b2Vec2 velocity = body->GetLinearVelocity();

  b2Vec2 lateral_velocity =
      b2Dot(lateral_direction, velocity) * lateral_direction;

  body->ApplyLinearImpulseToCenter(-1 * body->GetMass() * lateral_velocity,
                                   false);

  if (controlable) {
    b2Vec2 straight_direction = body->GetWorldVector({0, 1});
    body->ApplyForce(accelerate * 300.0f * straight_direction,
                     body->GetWorldCenter(), accelerate != 0);
  }
}

void Wheel::angular_velocity_tick(int turn_right) {
  turn_right = turn_right * 15;

  b2RevoluteJoint* rev_joint = (b2RevoluteJoint*)car_joint;

  if (turn_right != 0) {
    double left_dist = std::abs(rev_joint->GetJointAngle() - rev_joint->GetLowerLimit());
    double right_dist = std::abs(rev_joint->GetJointAngle() - rev_joint->GetUpperLimit());

    if (left_dist < 0.001) {
      turn_right = std::max(0, turn_right);
    }
  
    if (right_dist < 0.001) {
      turn_right = std::min(0, turn_right);
    }
  }

  body->SetAngularVelocity(turn_right);
}

Car::Car(b2World* world, b2Body* ground, b2Vec2 position) {
  // set up car body
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position = position;
  hull = world->CreateBody(&body_def);
  setup_hull();
  // set up wheels
  wheel[FRONT_LEFT] =
      new Wheel(world, ground, position + b2Vec2{-1.5, -1.5}, true);
  wheel[FRONT_RIGHT] =
      new Wheel(world, ground, position + b2Vec2{1.5, -1.5}, true);
  wheel[REAR_LEFT] =
      new Wheel(world, ground, position + b2Vec2{-2, -7.5}, false);
  wheel[REAR_RIGHT] =
      new Wheel(world, ground, position + b2Vec2{2, -7.5}, false);

  float sum_mass = hull->GetMass();
  for (Wheel* w : wheel) {
    sum_mass += w->body->GetMass();
  }

  for (Wheel* w : wheel) {
    w->set_friction(sum_mass / 4, friciton_coefficient);
  }

  // set up joints

  // front wheels
  b2RevoluteJointDef front_joint_def;

  front_joint_def.enableLimit = true;
  front_joint_def.lowerAngle = -steering_angle;
  front_joint_def.upperAngle = steering_angle;

  front_joint_def.Initialize(hull, wheel[FRONT_LEFT]->body,
                             wheel[FRONT_LEFT]->body->GetWorldCenter());
  wheel[FRONT_LEFT]->car_joint =
      (b2RevoluteJoint*)world->CreateJoint(&front_joint_def);

  front_joint_def.Initialize(hull, wheel[FRONT_RIGHT]->body,
                             wheel[FRONT_RIGHT]->body->GetWorldCenter());
  wheel[FRONT_RIGHT]->car_joint =
      (b2RevoluteJoint*)world->CreateJoint(&front_joint_def);

  // rear wheels
  b2WeldJointDef rear_joint_def;

  rear_joint_def.Initialize(hull, wheel[REAR_LEFT]->body,
                            wheel[REAR_LEFT]->body->GetWorldCenter());
  wheel[REAR_LEFT]->car_joint =
      (b2WeldJoint*)world->CreateJoint(&rear_joint_def);

  rear_joint_def.Initialize(hull, wheel[REAR_RIGHT]->body,
                            wheel[REAR_RIGHT]->body->GetWorldCenter());
  wheel[REAR_LEFT]->car_joint =
      (b2WeldJoint*)world->CreateJoint(&rear_joint_def);
}

void Car::setup_hull() {
  b2PolygonShape tip, middle, bottom;
  b2Vec2 tip_vec[3] = {{0, 0}, {-1.5, -1.5}, {1.5, -1.5}};
  b2Vec2 middle_vec[4] = {{1.5, -1.5}, {-1.5, -1.5}, {-1.25, -5}, {1.25, -5}};
  b2Vec2 bottom_vec[4] = {{1.25, -5}, {-1.25, -5}, {-2, -7.5}, {2, -7.5}};

  tip.Set(tip_vec, 3);
  middle.Set(middle_vec, 4);
  bottom.Set(bottom_vec, 4);

  hull->CreateFixture(&tip, density);
  hull->CreateFixture(&middle, density);
  hull->CreateFixture(&bottom, density);
}

b2Vec2 Car::pos() { return hull->GetWorldPoint({0, 0}); }

float Car::angle() { return hull->GetAngle(); }

float Car::wheel_angle(WheelID id) {
  return wheel[id]->body->GetAngle() - hull->GetAngle();
}

void Car::tick(std::unordered_set<Action> const& actions) {
  for (Wheel* w : wheel) {
    w->tick(actions);
  }
}

}  // namespace ph