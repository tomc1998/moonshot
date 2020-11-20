#pragma once

#include "constants.hpp"
#include "editor_state.hpp"
#include "globals.hpp"
#include "picker_item.hpp"

/** Call f with each picker item & the index, e.g. f(ii, picker_item) */
template <typename F> inline void picker_items_iter(EditorState &state, F f) {
  for (int ii = 0; ii < state.tileset->tiles.size(); ++ii) {
    f(ii, PickerItem(TileId(ii)));
  }
}

inline void update_picker(EditorState &state) {
  auto [mx, my] = GetMousePosition();
  bool mouse_over_picker = my > screen_h - PICKER_HEIGHT;
  if (mouse_over_picker && IsMouseButtonPressed(0)) {
    picker_items_iter(state, [&](int ii, const PickerItem &pi) {
      // Find the rendered tile position in the picker, this is the same
      // calculation as used when rendering the picker
      const Rectangle rect{
          PICKER_SPACING + ii * (PICKER_TILE_SIZE + PICKER_SPACING),
          screen_h - PICKER_TILE_SIZE - 32, PICKER_TILE_SIZE, PICKER_TILE_SIZE};
      // Check if the mouse pos is inside this dst rect
      if (CheckCollisionPointRec({mx, my}, rect)) {
        pi.select(state);
        return;
      }
    });
  }
}

inline void render_picker(EditorState &state) {
  DrawRectangle(0, screen_h - PICKER_HEIGHT, screen_w, PICKER_HEIGHT, BLACK);
  DrawLine(0, screen_h - PICKER_HEIGHT, screen_w, screen_h - PICKER_HEIGHT,
           UI_EDGE_COL);
  DrawText("Picker", 16, screen_h - PICKER_HEIGHT + 16, 20, UI_EDGE_COL);
  picker_items_iter(state, [&](int ii, const PickerItem &pi) {
    pi.render(state, PICKER_SPACING + ii * (PICKER_TILE_SIZE + PICKER_SPACING),
              screen_h - PICKER_TILE_SIZE - 32);
  });
}

