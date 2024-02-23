// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "test.h"
#include "tests/nandor/physics/car.h"

b2Body* create_ground(b2World* world) {
  b2BodyDef body_def;
  body_def.type = b2_staticBody;
  body_def.position = {0, 0};
  return world->CreateBody(&body_def);
}

class PhysicsCar : public Test {
 public:
  PhysicsCar()
      : ground{create_ground(m_world)},
        car{m_world, ground, {10, 10}} {}  // do nothing, no scene yet

  void Step(Settings& settings) {
    car.tick();
    Test::Step(settings);
  }

  b2Body* ground;
  Car car;
  static Test* Create() { return new PhysicsCar; }
};

static int testIndex =
    RegisterTest("nandor", "physics_test", PhysicsCar::Create);
