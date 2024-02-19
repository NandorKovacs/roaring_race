#include <box2d/b2_body.h>
#include <box2d/b2_friction_joint.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <cmath>

float gravity = 9.81;
float friciton_coefficient = 0.9; // asphalt, according to top google hit

// represents a car wheel.
class Wheel {
 public:
  Wheel(b2World* world, b2Body* ground, b2Vec2 position, bool controlable)
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

  void set_friction(float mass, float coeff) {
    friction_joint->SetMaxForce(mass * gravity * coeff);
    friction_joint->SetMaxTorque(mass * gravity * coeff * b2Distance({0,0}, {width/2, length/2}));
  }

  void tick() {
    lateral_velocity_tick();
    angular_velocity_tick();

    if (controlable) {
      control_tick();
    }
  }

  float get_mass() {
    return body->GetMass();
  }

 private:
  const float width = 0.5, length = 1.0, density = 1.0;
  bool controlable;

  b2Body* body;
  b2FrictionJoint* friction_joint;

  void lateral_velocity_tick() {
    b2Vec2 lateral_direction = body->GetWorldVector({0, 1});
    b2Vec2 velocity = body->GetLinearVelocity();

    b2Vec2 lateral_velocity =
        b2Dot(lateral_direction, velocity) * lateral_direction;

    body->ApplyLinearImpulseToCenter(-1 * body->GetMass() * lateral_velocity,
                                     false);
  }

  void angular_velocity_tick() { body->SetAngularVelocity(0.0); }

  void control_tick() {}
};

enum WheelID { FRONT_LEFT, FRONT_RIGHT, REAR_LEFT, REAR_RIGHT };

class Car {
 public:
  Car(b2World* world, b2Body* ground, b2Vec2 position) {
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
      sum_mass += w->get_mass();
    }
  
    for (Wheel* w : wheel) {
      w->set_friction(sum_mass / 4, friciton_coefficient);
    }
  
    // set up joints
    
  }

  void tick() {
    for (Wheel* w : wheel) {
      w->tick();
    }
  }

 private:
  const float density = 1.0;

  b2Body* hull;

  // front left, front right,
  Wheel* wheel[4];

  void setup_hull() {
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
};

int main() {}