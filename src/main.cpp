#include "engine.hpp"

int main() {
  engine::initialize("Snake", 800, 800);
  engine::engine_run();
  engine::shutdown();

  return 0;
}