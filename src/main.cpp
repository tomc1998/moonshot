#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <vector>

#include "constants.hpp"
#include "editor_screen.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include "game_screen.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "screen.hpp"
#include "screen_stack.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"

int main(void) {
  InitWindow(screen_w, screen_h, "window_me");
  SetTargetFPS((int)FPS);

  const auto tileset = load_simple_tileset();
  const Level level_1{
      tileset,
      // clang-format off
    Tilemap{0, 0, 16, 16, {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }},
      // clang-format on
      {
          Entity(EK_PLAYER, 16, 16),
          Entity(EK_MIRROR, 16, 64, MirrorData{PI / 4}),
          Entity(EK_MIRROR, 64, 64, MirrorData{-PI / 4}),
          Entity(EK_MIRROR, 64, 16, MirrorData{-PI / 4}),
          Entity(EK_MIRROR, 128, 16, MirrorData{PI / 3}),
      },
      {24, 24}};

  // Disable raylib exiting when <esc> is pressed
  SetExitKey(0);

  ScreenStack screen_stack;
  screen_stack.push(std::make_unique<EditorScreen>());
  screen_stack.push(std::make_unique<GameScreen>());

  level_1.load(reinterpret_cast<GameScreen &>(screen_stack.last()).state);

  while (!WindowShouldClose()) {
    screen_stack.last().on_frame();
    if (IsKeyPressed(KEY_ESCAPE)) {
      screen_stack.pop();
      if (screen_stack.size() == 0) {
        break;
      }
    }
  }

  CloseWindow();
  return 0;
}

