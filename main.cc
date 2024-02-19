#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_friction_joint.h>

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
    shape.SetAsBox(width, height);

    body = world->CreateBody(&body_def);

    // currently no need to save fixture, retrive here if needed
    body->CreateFixture(&shape, density);

    // create friction joint to ground object
    b2FrictionJointDef joint_def;
    joint_def.bodyA = ground;
    joint_def.bodyB = body;
    joint_def.localAnchorA.SetZero();
    joint_def.localAnchorB = body->GetLocalCenter();
    
  }

 private:
  const float width = 0.5, height = 1.0, density = 1.0;
  bool controlable;
  b2Body* body;
};

int main() {}