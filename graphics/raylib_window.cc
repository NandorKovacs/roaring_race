#include "raylib_window.h"

#include "raylib.h"
#include "raymath.h"

namespace gui {

GameWindow::GameWindow() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1600, 1200, "roaring race");
  SetTargetFPS(60);

  camera.target = {0, 0};
  camera.offset = {800, 600};

  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
}

void GameWindow::tick() {
  if (WindowShouldClose()) {
    CloseWindow();
  }

  camera.target = {0, 0};
  camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

  BeginDrawing();
  {
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    {
      for (Drawable* d : drawables) {
        d->draw();
      }
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
}

void DrawableCar::draw() {
  std::vector<Vector2> new_triangles = triangles;

  Matrix translate =
      MatrixTranslate(state.position.first, state.position.second, 0.0f);
  Matrix rotate = MatrixRotate({0.0f, 0.0f, 1.0f}, state.angle);

  Matrix car_mat = MatrixMultiply(translate, rotate);

  for (Vector2& v : new_triangles) {
    v = Vector2Transform(v, car_mat);
  }

  std::array<Rectangle, 4> wheels;
  for (int i = 0; i < 4; ++i) {
    wheels[i].height = 1;
    wheels[i].width = 0.5;

    Vector2 joint = Vector2Transform(wheel_joint[i], car_mat);

    wheels[i].x = joint.x - (wheels[i].width / 2);
    wheels[i].y = joint.y + (wheels[i].height / 2);
  }

  DrawTriangleStrip(new_triangles.data(), new_triangles.size(), LIME);
  for (int i = 0; i < 4; ++i) {
    Rectangle w = wheels[i];
    DrawRectanglePro(w, {w.width / 2, w.height / 2}, state.wheel_angle[i],
                     BLACK);
  }
}

}  // namespace gui