#pragma once

#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "camera_util.hpp"
#include "editor_state.hpp"
#include "globals.hpp"
#include "screen.hpp"
#include "simple_tileset.hpp"

struct EditorScreen : public Screen {
  EditorState state;
  Camera2D camera;

  EditorScreen() {
    camera.offset = (Vector2){(float)screen_w / 2, (float)screen_h / 2};
    camera.rotation = 0;
    camera.target = {16, 16};
    camera.zoom = 2;
  }

  void on_mount() override { state.tileset = load_simple_tileset(); }

  void on_frame() {
    const auto &ts = *state.tileset;

    auto mouse_world = screen_to_world(camera, GetMousePosition());
    int tile_hover_x = (int)floor(mouse_world.x / (float)ts.tile_size);
    int tile_hover_y = (int)floor(mouse_world.y / (float)ts.tile_size);

    if (IsMouseButtonPressed(0)) {
      state.erasing = state.tiles.get(tile_hover_x, tile_hover_y) != nullptr;
    }
    if (IsMouseButtonDown(0)) {
      if (state.erasing) {
        state.tiles.remove(tile_hover_x, tile_hover_y);
      } else {
        state.tiles.set(tile_hover_x, tile_hover_y, 1);
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);

    DrawRectangle(100, 100, 16, 16, BLUE);

    // Render tiles
    for (const auto &t : state.tiles) {
      const auto &tile = ts.tiles[t.val];
      const Rectangle tile_rect{(float)tile.x * ts.tile_size,
                                (float)tile.y * ts.tile_size,
                                (float)ts.tile_size, (float)ts.tile_size};
      DrawTextureRec(ts.tex, tile_rect,
                     {(float)t.x * ts.tile_size, (float)t.y * ts.tile_size},
                     WHITE);
    }

    // Draw a transparent rectangle over where the tile is hovered
    DrawRectangle(tile_hover_x * ts.tile_size, tile_hover_y * ts.tile_size,
                  ts.tile_size, ts.tile_size, {255, 255, 255, 150});

    EndMode2D();

    DrawFPS(10, screen_h - 26);
    EndDrawing();
  }
};
