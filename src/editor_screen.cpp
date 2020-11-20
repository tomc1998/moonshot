#include "editor_screen.hpp"

#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "camera_util.hpp"
#include "editor_picker.hpp"
#include "editor_state.hpp"
#include "game_screen.hpp"
#include "globals.hpp"
#include "picker_item.hpp"
#include "render_game.hpp"
#include "screen.hpp"
#include "screen_stack.hpp"
#include "simple_tileset.hpp"

EditorScreen::EditorScreen() {
  camera.offset = (Vector2){(float)screen_w / 2, (float)screen_h / 2};
  camera.rotation = 0;
  camera.target = {16, 16};
  camera.zoom = 2;
}

void EditorScreen::on_mount() { state.tileset = load_simple_tileset(); }

void EditorScreen::test(ScreenStack &stack) {
  Level level = state.serialise();
  stack.push(std::make_unique<GameScreen>());
  level.load(reinterpret_cast<GameScreen &>(stack.last()).state);
}

void EditorScreen::on_frame(ScreenStack &stack) {
  const auto &ts = *state.tileset;

  // Get mouse screen / world position
  auto [mx, my] = GetMousePosition();
  auto [mwx, mwy] = screen_to_world(camera, {mx, my});
  int tile_hover_x = (int)floor(mwx / (float)ts.tile_size);
  int tile_hover_y = (int)floor(mwy / (float)ts.tile_size);

  // Is the mouse over the picker?
  bool mouse_over_picker = my > screen_h - PICKER_HEIGHT;

  // Load level?
  if (IsKeyPressed(KEY_F5)) {
    test(stack);
    return;
  }

  update_picker(state);

  // Process mouse click / release
  if (IsMouseButtonPressed(0)) {
    if (!mouse_over_picker) {
      state.drawing = true;
      state.erasing = state.tiles.get(tile_hover_x, tile_hover_y) != nullptr;
    }
  } else if (IsMouseButtonReleased(0)) {
    state.drawing = false;
  }
  // Pipette a tile
  if (IsMouseButtonPressed(1)) {
    if (!mouse_over_picker) {
      const auto tile = state.tiles.get(tile_hover_x, tile_hover_y);
      if (tile) {
        state.picker_item = PickerItem(*tile);
      }
    }
  }

  // Draw / erase tiles
  if (state.drawing) {
    switch (state.picker_item.kind) {
    case PK_TILE: {
      if (state.erasing || state.picker_item.tile == 0) {
        state.tiles.remove(tile_hover_x, tile_hover_y);
      } else {
        state.tiles.set(tile_hover_x, tile_hover_y,
                        std::move(state.picker_item.tile));
      }
      break;
    }
    case PK_ENTITY:
      assert(false && "Unimpl Entity");
    case PK_LASER:
      state.laser_pos = {
          (float)tile_hover_x * ts.tile_size + (float)ts.tile_size / 2.0f,
          (float)tile_hover_y * ts.tile_size + (float)ts.tile_size / 2.0f};
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
                              (float)tile.y * ts.tile_size, (float)ts.tile_size,
                              (float)ts.tile_size};
    DrawTextureRec(ts.tex, tile_rect,
                   {(float)t.x * ts.tile_size, (float)t.y * ts.tile_size},
                   WHITE);
  }

  // Render laser base
  render_laser_base(state.laser_pos);

  // Draw a transparent rectangle over where the tile is hovered
  DrawRectangle(tile_hover_x * ts.tile_size, tile_hover_y * ts.tile_size,
                ts.tile_size, ts.tile_size, {255, 255, 255, 150});

  EndMode2D();

  render_picker(state);

  DrawFPS(10, screen_h - 26);
  EndDrawing();
}
