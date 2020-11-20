#include "editor_picker.hpp"
#include "editor_state.hpp"
#include <cstring>

PickerItem::PickerItem(TileId tile) : kind(PK_TILE), tile(tile) {}
PickerItem::PickerItem(EntityKind entity) : kind(PK_ENTITY), entity(entity) {}
PickerItem::PickerItem(PickerItemKind kind) : kind(kind) {}

void PickerItem::select(EditorState &state) const { state.picker_item = *this; }

void PickerItem::render(const EditorState &state, float x, float y) const {
  const auto &ts = *state.tileset;
  switch (kind) {
  case PK_TILE: {
    const auto &tile = ts.tiles[this->tile];
    const Rectangle tile_rect{(float)tile.x * ts.tile_size,
                              (float)tile.y * ts.tile_size, (float)ts.tile_size,
                              (float)ts.tile_size};
    const Rectangle dst_rect{x, y, PICKER_TILE_SIZE, PICKER_TILE_SIZE};
    DrawTexturePro(ts.tex, tile_rect, dst_rect, {0, 0}, 0, WHITE);
    const int line_thickness = (state.picker_item == *this) ? 4 : 1;
    DrawRectangleLinesEx(dst_rect, line_thickness, UI_EDGE_COL);
    break;
  }
  case PK_ENTITY:
    break;
  case PK_LASER:
    break;
  }
}

bool PickerItem::operator==(const PickerItem &other) const {
  return memcmp(this, &other, sizeof(PickerItem)) == 0;
}
