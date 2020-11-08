#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include "game_screen.hpp"
#include "game_state.hpp"
#include "globals.hpp"
#include "render_game.hpp"
#include "screen.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"
#include "update_game.hpp"
#include <cmath>
#include <raymath.h>

struct GameScreen : public Screen {
  GameState state;
  GameScreen() {}
  void on_frame() {
    game_update(state);

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(state.camera.inner);
    state.tm.render(*state.tileset);
    render_entities(state.es);
    render_laser(state.laser);
    EndMode2D();

    DrawFPS(10, screen_h - 26);
    EndDrawing();
  }
};
