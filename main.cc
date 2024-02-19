#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>

// represents a car wheel.
class Wheel {
 public:
  Wheel(b2World* world, b2Body* ground, b2Vec2 position, bool controlable)
      : controlable{controlable} {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position = position;

    b2PolygonShape shape;
    shape.SetAsBox(width, height);
    
  }

 private:
  const float width = 0.5, height = 1.0;
  bool controlable;
  b2Body* body;
};

int main() {}