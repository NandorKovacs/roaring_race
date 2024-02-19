#include <box2d/b2_body.h>
#include <box2d/b2_friction_joint.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <box2d/b2_revolute_joint.h>
#include <box2d/b2_weld_joint.h>
#include <cmath>

const float gravity = 9.81;
const float friciton_coefficient = 0.9; // asphalt, according to top google hit

// represents a car wheel.
class Wheel {
 public:
  b2Body* body;

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

 private:
  const float width = 0.5, length = 1.0, density = 1.0;
  bool controlable;

  
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
      sum_mass += w->body->GetMass();
    }
  
    for (Wheel* w : wheel) {
      w->set_friction(sum_mass / 4, friciton_coefficient);
    }
  
    // set up joints
    
    // front wheels
    b2RevoluteJointDef front_joint_def;
    front_joint_def.bodyA = hull;
    front_joint_def.enableLimit = true;
    front_joint_def.lowerAngle = -steering_angle;
    front_joint_def.upperAngle = steering_angle;

    front_joint_def.bodyB = wheel[FRONT_LEFT]->body;
    front_joint_def.localAnchorA = front_joint_def.localAnchorB = wheel[FRONT_LEFT]->body->GetWorldCenter();
    front_joint[FRONT_LEFT] = (b2RevoluteJoint*) world->CreateJoint(&front_joint_def);

    front_joint_def.bodyB = wheel[FRONT_RIGHT]->body;
    front_joint_def.localAnchorA = front_joint_def.localAnchorB = wheel[FRONT_RIGHT]->body->GetWorldCenter();
    front_joint[FRONT_RIGHT] = (b2RevoluteJoint*) world->CreateJoint(&front_joint_def);

    // rear wheels
    b2WeldJointDef rear_joint_def;
    rear_joint_def.bodyA = hull;

    rear_joint_def.bodyB = wheel[REAR_LEFT]->body;
    rear_joint_def.localAnchorA = rear_joint_def.localAnchorB = wheel[REAR_LEFT]->body->GetWorldCenter();
    rear_joint[REAR_LEFT - 2] = (b2WeldJoint*) world->CreateJoint(&rear_joint_def);

    rear_joint_def.bodyB = wheel[REAR_RIGHT]->body;
    rear_joint_def.localAnchorA = rear_joint_def.localAnchorB = wheel[REAR_RIGHT]->body->GetWorldCenter();
    rear_joint[REAR_RIGHT - 2] = (b2WeldJoint*) world->CreateJoint(&rear_joint_def);
  }

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

  b2Joint* get_joint(WheelID wheel_id) {
    if (wheel_id < 3) {
      return front_joint[wheel_id];
    }
    return rear_joint[wheel_id - 2];
  }

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

b2Body* create_ground(b2World* world) {
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position = {0,0};
  return world->CreateBody(&body_def);
}

int main() {
  b2World* world = new b2World({0,0});
  b2Body* body = create_ground(world);

  
}