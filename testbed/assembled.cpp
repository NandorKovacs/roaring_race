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
#include <iostream>

#include "test.h"
namespace assembled {

struct ControlState {
  bool accel = false, brake = false, left = false, right = false;

  void print() {
    std::cout << "{accel: " << accel << ", brake: " << brake << ", left: " << left << ", right: " << right << "}" << std::endl;
  }
};

class Wheel {
 public:
  b2Body* wheel;
  Wheel(b2World* world, b2Body* ground, b2Vec2 position) {
    {
      b2BodyDef wheel_body;
      wheel_body.type = b2_dynamicBody;
      wheel_body.position = position;

      b2PolygonShape poly;
      poly.SetAsBox(0.5, 1.0, {0, 0}, 0);

      wheel = world->CreateBody(&wheel_body);
      wheel->CreateFixture(&poly, 1.0f);
    }

    {
      b2FrictionJointDef jd;
      jd.bodyA = ground;
      jd.bodyB = wheel;
      jd.localAnchorA.SetZero();
      jd.localAnchorB = wheel->GetLocalCenter();
      jd.collideConnected = false;
      jd.maxForce = 1.0f * wheel->GetMass() * 10;
      jd.maxTorque = 0.7f * wheel->GetInertia() * 2 * 10;
      world->CreateJoint(&jd);
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

  void handle_controls(ControlState ctrl_state) {
    if (ctrl_state.accel != ctrl_state.brake) {
      b2Vec2 direction = wheel->GetWorldVector({0, 1});

      ctrl_state.print();

      if (ctrl_state.accel == true) {
        std::cout << "accel" << std::endl;
        wheel->ApplyForce(100.0f * direction, wheel->GetWorldCenter(), true);
      }
      if (ctrl_state.brake == true) {
        std::cout << "brake" << std::endl;
        wheel->ApplyForce(-50.0f * direction, wheel->GetWorldCenter(), true);
      }
    }

  }

  void update(ControlState ctrl_state) {
    remove_sideways_slide();
    remove_angular_velocity();
    handle_controls(ctrl_state);
  }
};

class Car {
 public:
  b2Body* car;

  Car(b2World* world, b2Vec2 position) {
    b2BodyDef car_body;
    car_body.type = b2_dynamicBody;
    car_body.position = position;

    car = world->CreateBody(&car_body);
    second_design();
  }

  void first_design() {
    b2PolygonShape hull;
    b2Vec2 points[6] = {{1, 2}, {3, 5}, {2, 9}, {-2, 9}, {-3, 5}, {-1, 2}};
    hull.Set(points, 6);

    b2PolygonShape tip;
    b2Vec2 tp[3] = {{0, 0}, {1, 2}, {-1, 2}};
    tip.Set(tp, 3);
    car->CreateFixture(&hull, 1.0f);
    car->CreateFixture(&tip, 1.0f);
  }

  void second_design() {
    b2PolygonShape tip, middle, bottom;
    b2Vec2 tip_vec[3] = {{0, 1.5}, {-1.5, 0}, {1.5, 0}};
    b2Vec2 middle_vec[4] = {{1.5, 0}, {-1.5, 0}, {-1.25, -3.5}, {1.25, -3.5}};
    b2Vec2 bottom_vec[4] = {{1.25, -3.5}, {-1.25, -3.5}, {-2, -6}, {2, -6}};

    tip.Set(tip_vec, 3);
    middle.Set(middle_vec, 4);
    bottom.Set(bottom_vec, 4);

    car->CreateFixture(&tip, 1.0f);
    car->CreateFixture(&middle, 1.0f);
    car->CreateFixture(&bottom, 1.0f);
  }
};

class WheeledCar : public Test {
 public:
  b2Body* ground;
  Wheel* wheels[4];
  Car* car;
  WheeledCar() {
    m_world->SetGravity(b2Vec2(0.0f, 0.0f));

    create_ground();

    create_wheels();

    create_car();

    fix_wheels();
  }

  void Step(Settings& settings) {
    update();

    Test::Step(settings);
  }

  static Test* Create() { return new WheeledCar; }

  void Keyboard(int key) {
    // std::cout << key << std::endl;
    switch (key) {
      case GLFW_KEY_W:
        ctrl_state.accel = true;
        break;
      case GLFW_KEY_A:
        ctrl_state.left = true;
        break;
      case GLFW_KEY_S:
        ctrl_state.brake = true;
        break;
      case GLFW_KEY_D:
        ctrl_state.right = true;
        break;

      default:
        Test::Keyboard(key);
    }
  }

  void KeyboardUp(int key) {
    switch (key) {
      case GLFW_KEY_W:
        ctrl_state.accel = false;
        break;
      case GLFW_KEY_A:
        ctrl_state.left = false;
        break;
      case GLFW_KEY_S:
        ctrl_state.brake = false;
        break;
      case GLFW_KEY_D:
        ctrl_state.right = false;
        break;

      default:
        Test::KeyboardUp(key);
    }
  }

 private:
  ControlState ctrl_state;

  void create_ground() {
    b2BodyDef ground_body;
    ground_body.type = b2_staticBody;
    ground_body.position = b2Vec2{10, 0};
    ground = m_world->CreateBody(&ground_body);

    b2CircleShape shape;
    shape.m_radius = 0.1;

    b2FixtureDef fix;
    fix.density = 0.0f;
    fix.shape = &shape;

    ground->CreateFixture(&fix);
  }

  void create_wheels() {
    wheels[0] = new Wheel(m_world, ground, {-1.5, 0});
    wheels[1] = new Wheel(m_world, ground, {1.5, 0});
    wheels[2] = new Wheel(m_world, ground, {-2, -6});
    wheels[3] = new Wheel(m_world, ground, {2, -6});
  }

  void create_car() { car = new Car(m_world, {0, 0}); }

  void fix_wheels() {
    b2RevoluteJointDef fl, fr;
    fl.Initialize(car->car, wheels[0]->wheel,
                  car->car->GetWorldPoint({-1.5, 0}));
    fr.Initialize(car->car, wheels[1]->wheel,
                  car->car->GetWorldPoint({1.5, 0}));

    fl.enableLimit = true;
    fr.enableLimit = true;

    double steering_angle = b2_pi * 5 / 32;

    fl.lowerAngle = -steering_angle;
    fl.upperAngle = steering_angle;
    fr.lowerAngle = -steering_angle;
    fr.upperAngle = steering_angle;

    b2WeldJointDef bl, br;
    bl.Initialize(car->car, wheels[2]->wheel,
                  car->car->GetWorldPoint({-2, -6}));
    br.Initialize(car->car, wheels[3]->wheel, car->car->GetWorldPoint({2, -6}));

    m_world->CreateJoint(&fl);
    m_world->CreateJoint(&fr);
    m_world->CreateJoint(&bl);
    m_world->CreateJoint(&br);
  }

  void update() {
    for (int i = 0; i < 4; ++i) {
      wheels[i]->update(ctrl_state);
    }
  }
};
}  // namespace assembled

static int testIndex =
    RegisterTest("nandor", "wheeled_car", assembled::WheeledCar::Create);
