#include "editor_picker.hpp"
#include "editor_state.hpp"
#include <cstring>

PickerItem::PickerItem(TileId tile) : kind(PK_TILE), tile(tile) {}
PickerItem::PickerItem(EntityKind entity) : kind(PK_ENTITY), entity(entity) {}
PickerItem::PickerItem(PickerItemKind kind) : kind(kind) {}

void PickerItem::select(EditorState &state) const { state.picker_item = *this; }

void PickerItem::render(const EditorState &state, float x, float y) const {
  const auto &ts = *state.tileset;
  const Rectangle dst_rect{x, y, PICKER_TILE_SIZE, PICKER_TILE_SIZE};
  switch (kind) {
  case PK_TILE: {
    const auto &tile = ts.tiles[this->tile];
    const Rectangle tile_rect{(float)tile.x * ts.tile_size,
                              (float)tile.y * ts.tile_size, (float)ts.tile_size,
                              (float)ts.tile_size};
    DrawTexturePro(ts.tex, tile_rect, dst_rect, {0, 0}, 0, WHITE);
    break;
  }
  case PK_ENTITY:
    break;
  case PK_LASER:
    auto laser_txt_size = MeasureTextEx(GetFontDefault(), "Laser", 10, 1);
    auto text_pos = Vector2Subtract(
        Vector2Add({dst_rect.x, dst_rect.y},
                   Vector2Scale({dst_rect.width, dst_rect.height}, 0.5)),
        Vector2Scale(laser_txt_size, 0.5));
    DrawTextEx(GetFontDefault(), "Laser", text_pos, 10, 1.0, WHITE);
    break;
  }
  const int line_thickness = (state.picker_item == *this) ? 4 : 1;
  DrawRectangleLinesEx(dst_rect, line_thickness, UI_EDGE_COL);
}

bool PickerItem::operator==(const PickerItem &other) const {
  return memcmp(this, &other, sizeof(PickerItem)) == 0;
}
