#include <raylib.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "constants.hpp"
#include "editor_screen.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include "game_screen.hpp"
#include "globals.hpp"
#include "screen.hpp"
#include "screen_stack.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"

int main(void) {
  InitWindow(screen_w, screen_h, "window_me");
  SetTargetFPS((int)FPS);

  // Disable raylib exiting when <esc> is pressed
  SetExitKey(0);

  ScreenStack screen_stack;
  screen_stack.push(std::make_unique<EditorScreen>());
  screen_stack.push(std::make_unique<GameScreen>());

  while (!WindowShouldClose()) {
    screen_stack.last().on_frame();
  }

  CloseWindow();
  return 0;
}

