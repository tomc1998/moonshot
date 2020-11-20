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

/* Just some UI constants defining the size / color of the UI widgets: */
/** Height of the picker (displayed along bottom of screen) */
constexpr float PICKER_HEIGHT = 128.0;
/** Scale of tiles in the picker */
constexpr float PICKER_SCALE = 2.0;
/** Spacing between tiles in the picker */
constexpr float PICKER_SPACING = 16.0;
/** The 'edge color' used across the UI to accentuate UI elements and make them
 * stick out against other screen elements. */
constexpr Color UI_EDGE_COL{0, 200, 255, 255};

/** Check the mouse position, change the state's current position if the mouse
 * is currently over a tile in the picker. */
inline void pick_tile(EditorState &state, Vector2 mouse) {
  const auto &ts = *state.tileset;
  for (int ii = 0; ii < ts.tiles.size(); ++ii) {
    const float picker_tile_size = (float)ts.tile_size * PICKER_SCALE;
    // Find the rendered tile position in the picker, this is the same
    // calculation as used when rendering the picker
    const Rectangle rect{
        PICKER_SPACING + ii * (picker_tile_size + PICKER_SPACING),
        screen_h - picker_tile_size - 32, picker_tile_size, picker_tile_size};
    // Check if the mouse pos is inside this dst rect
    if (CheckCollisionPointRec(mouse, rect)) {
      state.curr_tile = ii;
      return;
    }
  }
}

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

    // Get mouse screen / world position
    auto [mx, my] = GetMousePosition();
    auto [mwx, mwy] = screen_to_world(camera, {mx, my});
    int tile_hover_x = (int)floor(mwx / (float)ts.tile_size);
    int tile_hover_y = (int)floor(mwy / (float)ts.tile_size);

    // Is the mouse over the picker?
    bool mouse_over_picker = my > screen_h - PICKER_HEIGHT;

    // Process mouse click / release
    if (IsMouseButtonPressed(0)) {
      if (mouse_over_picker) {
        pick_tile(state, {mx, my});
      } else {
        state.drawing_tiles = true;
        state.erasing = state.tiles.get(tile_hover_x, tile_hover_y) != nullptr;
      }
    } else if (IsMouseButtonReleased(0)) {
      state.drawing_tiles = false;
    }

    // Draw / erase tiles
    if (state.drawing_tiles) {
      if (state.erasing) {
        state.tiles.remove(tile_hover_x, tile_hover_y);
      } else {
        state.tiles.set(tile_hover_x, tile_hover_y, state.curr_tile);
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

    // Render picker
    DrawRectangle(0, screen_h - PICKER_HEIGHT, screen_w, PICKER_HEIGHT, BLACK);
    DrawLine(0, screen_h - PICKER_HEIGHT, screen_w, screen_h - PICKER_HEIGHT,
             UI_EDGE_COL);
    DrawText("Picker", 16, screen_h - PICKER_HEIGHT + 16, 20, UI_EDGE_COL);
    for (int ii = 0; ii < ts.tiles.size(); ++ii) {
      const auto &tile = ts.tiles[ii];
      const float picker_tile_size = (float)ts.tile_size * PICKER_SCALE;
      const Rectangle tile_rect{(float)tile.x * ts.tile_size,
                                (float)tile.y * ts.tile_size,
                                (float)ts.tile_size, (float)ts.tile_size};
      const Rectangle dst_rect{
          PICKER_SPACING + ii * (picker_tile_size + PICKER_SPACING),
          screen_h - picker_tile_size - 32, picker_tile_size, picker_tile_size};
      DrawTexturePro(ts.tex, tile_rect, dst_rect, {0, 0}, 0, WHITE);
      const int line_thickness = (state.curr_tile == ii) ? 4 : 1;
      DrawRectangleLinesEx(dst_rect, line_thickness, UI_EDGE_COL);
    }

    DrawFPS(10, screen_h - 26);
    EndDrawing();
  }
};
