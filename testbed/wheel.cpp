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

class Wheel : public Test {
 public:
  b2Body* wheel;
  b2Body* ground;
  Wheel() {
    m_world->SetGravity(b2Vec2(0.0f, 0.0f));

    {
      b2BodyDef wheel_body;
      wheel_body.type = b2_dynamicBody;
      wheel_body.position = b2Vec2{0, 0};

      b2PolygonShape poly;
      poly.SetAsBox(0.3, 0.6, {0, 0}, 0);

      wheel = m_world->CreateBody(&wheel_body);
      wheel->CreateFixture(&poly, 1.0f);
    }

    {
      b2BodyDef ground_body;
      ground_body.type = b2_staticBody;
      ground_body.position = b2Vec2{10, 0};
      ground = m_world->CreateBody(&ground_body);

      b2CircleShape c;
      c.m_radius = 1;

      b2FixtureDef fix;
      fix.density = 0.0f;
      fix.shape = &c;

      ground->CreateFixture(&fix);
    }

    {
      b2FrictionJointDef jd;
      jd.bodyA = ground;
      jd.bodyB = wheel;
      jd.localAnchorA.SetZero();
      jd.localAnchorB = wheel->GetLocalCenter();
      jd.collideConnected = true;
      jd.maxForce = 0.7f * wheel->GetMass() * 10;
      jd.maxTorque = 0.5f * wheel->GetInertia() * 2 * 10;
      m_world->CreateJoint(&jd);
    }
  }

  void remove_sideways_slide() {
    b2Vec2 side_vec = wheel->GetWorldVector({1, 0});
    b2Vec2 v = wheel->GetLinearVelocity();

    b2Vec2 sidev = b2Dot(side_vec, v) * side_vec;

    wheel->ApplyLinearImpulse(wheel->GetMass() * -sidev,
                              wheel->GetWorldCenter(), false);
  }

  void remove_angular_velocity() { wheel->SetAngularVelocity(0.0f); }

  void update() {
    remove_sideways_slide();
    remove_angular_velocity();
  }

  void Step(Settings& settings) {
    update();

    Test::Step(settings);
  }

  static Test* Create() { return new Wheel; }
};

static int testIndex = RegisterTest("nandor", "wheel", Wheel::Create);
