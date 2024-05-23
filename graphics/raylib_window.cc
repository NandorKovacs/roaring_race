#include "raylib_window.h"

#include <iostream>

#include "raylib.h"
#include "raymath.h"

namespace gui {

GameWindow::GameWindow() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  // SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(1600, 1200, "roaring race");
  SetTargetFPS(60);

  camera.target = {0, 0};

  int monitor = GetCurrentMonitor();
  double ppmm = GetMonitorHeight(monitor) / GetMonitorPhysicalHeight(monitor);

  default_zoom = ppmm / TARGET_PPMM;

  camera.zoom = default_zoom;

  camera.rotation = 0.0f;
}

void GameWindow::tick() {
  if (WindowShouldClose()) {
    CloseWindow();
  }

  camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

  BeginDrawing();
  {
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    {
      // std::cout << "-----" << std::endl;
      for (Drawable* d : drawables) {
        d->draw();
      }
      // std::cout << "-----" << std::endl;
    }
    EndMode2D();
  }
  EndDrawing();
}

bool GameWindow::is_open() { return IsWindowReady(); };
void GameWindow::close() { CloseWindow(); };

DrawableCar::DrawableCar() {
  triangles = {{0, 0},       {-1.5, -1.5}, {1.5, -1.5}, {1.5, -1.5},
               {-1.5, -1.5}, {-1.25, -5},  {1.5, -1.5}, {-1.25, -5},
               {1.25, -5},   {1.25, -5},   {-1.25, -5}, {-2, -7.5},
               {1.25, -5},   {-2, -7.5},   {2, -7.5}};
  wheel_joint = {Vector2{-1.5, -1.5}, Vector2{1.5, -1.5}, Vector2{-2, -7.5},
                 Vector2{2, -7.5}};
  wheel_size = {0.5, 1};

  for (auto& x : triangles) {
    x = {x.x * 10, x.y * 10};
  }
  for (auto& x : wheel_joint) {
    x = {x.x * 10, x.y * 10};
  }
  wheel_size = {wheel_size.x * 10, wheel_size.y * 10};
}

void DrawableCar::draw() {
  std::vector<Vector2> new_triangles = triangles;

  Matrix translate = MatrixTranslate(state.position.first * 10,
                                     state.position.second * 10, 0.0f);
  Matrix rotate = MatrixRotate({0.0f, 0.0f, 1.0f}, state.angle);

  Matrix car_mat = MatrixMultiply(rotate, translate);

  for (Vector2& v : new_triangles) {
    v = Vector2Transform(v, car_mat);
  }

  std::array<Rectangle, 4> wheels;
  for (int i = 0; i < 4; ++i) {
    wheels[i].height = wheel_size.y;
    wheels[i].width = wheel_size.x;

    Vector2 joint = Vector2Transform(wheel_joint[i], car_mat);
    wheels[i].x = joint.x;
    wheels[i].y = joint.y;
  }

  // DrawTriangleStrip(new_triangles.data(), new_triangles.size(), LIME);

  for (int i = 2; i < new_triangles.size(); i += 3) {
    DrawTriangle(new_triangles[i], new_triangles[i - 1], new_triangles[i - 2],
                 GREEN);
  }

  for (int i = 0; i < 4; ++i) {
    Rectangle w = wheels[i];
    // std::cout << w.width << " " << w.height << " " << w.x <<  " " << w.y <<
    // std::endl;

    double angle = state.angle;
    if (i < 2) {
      angle += state.wheel_angle[i];
    }
    DrawRectanglePro(w, {w.width / 2, w.height / 2}, RAD2DEG * angle, BLACK);
  }
}

}  // namespace gui