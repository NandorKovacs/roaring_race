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

class Car2d : public Test {
 public:
  Car2d() {
    m_world->SetGravity(b2Vec2(0.0f, 0.0f));
    {
      b2BodyDef car_body;
      car_body.type = b2_dynamicBody;
      car_body.position = b2Vec2{0, 0};
      
      b2PolygonShape hull;
      b2Vec2 points[6] = {{1,2}, {3,5}, {2,9}, {-2,9}, {-3,5}, {-1,2}};
      hull.Set(points, 6);

      b2PolygonShape tip;
      b2Vec2 tp[3] = {{0,0}, {1,2}, {-1,2}};
      tip.Set(tp, 3);
      
      b2Body* car = m_world->CreateBody(&car_body);
      car->CreateFixture(&hull, 1.0f);
      car->CreateFixture(&tip, 1.0f);
    }

  }
  static Test* Create() { return new Car2d; }
};

static int testIndex = RegisterTest("nandor", "Car2d", Car2d::Create);
