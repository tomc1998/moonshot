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
#include "navmesh.hpp"
#include "screen.hpp"
#include "screen_stack.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"

int main(void) {
  InitWindow(screen_w, screen_h, "window_me");
  SetTargetFPS((int)FPS);

  const auto tileset = load_simple_tileset();
  Level level_1{
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
          Entity(EK_ENEMY_BASIC, 32, 32,
                 EnemyBasicData{
                     {EnemyBasicAction({48, 32}), EnemyBasicAction(1),
                      EnemyBasicAction({48, 48}), EnemyBasicAction(1),
                      EnemyBasicAction({32, 48}), EnemyBasicAction(1),
                      EnemyBasicAction({32, 32}), EnemyBasicAction(1)}}),
      },
      {24, 24}};

  // Unfortunately since the construction of the entity data for an enemy along
  // a path relies on knowing the NavMesh, which in turn is constructed from the
  // Tilemap and Tileset, we need to add those after the level has already been
  // created (and make the level non-const). It's probably possible with
  // refactoring and addition of more constructors to get around this.
  //
  // Here we're adding an enemy that moves on paths from tiles (3,12) ->
  // (11,6) -> (5,1)
  NavMesh nm(level_1.tilemap, *level_1.tileset);
  level_1.entity_list.emplace_back(
      EK_ENEMY_BASIC, Tilecoords{5, 1},
      EnemyPathData{{EnemyPathAction({3, 12}), EnemyPathAction(3),
                     EnemyPathAction({11, 6}), EnemyPathAction(1),
                     EnemyPathAction({13, 8}), EnemyPathAction(3),
                     EnemyPathAction({5, 1}), EnemyPathAction(2)}},
      nm);

  // Disable raylib exiting when <esc> is pressed
  SetExitKey(0);

  ScreenStack screen_stack;
  screen_stack.push(std::make_unique<EditorScreen>());
  screen_stack.push(std::make_unique<GameScreen>());

  level_1.load(reinterpret_cast<GameScreen &>(screen_stack.last()).state);

  while (!WindowShouldClose()) {
    screen_stack.last().on_frame(screen_stack);
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
