#include "window.h"

GameWindow::GameWindow() : window(sf::VideoMode(800, 600), "roaring race") {}

void GameWindow::tick() {
  window.clear();
}