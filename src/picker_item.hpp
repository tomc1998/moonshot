#pragma once

#include "entity.hpp"
#include "tilemap.hpp"

class EditorState;

enum PickerItemKind { PK_TILE, PK_ENTITY, PK_LASER };
struct PickerItem {
  PickerItemKind kind;
  union {
    /** PK_TILE */
    TileId tile;
    /** PK_ENTITY */
    EntityKind entity;
  };
  PickerItem(TileId tile);
  PickerItem(EntityKind entity);
  PickerItem(PickerItemKind kind);
  void render(const EditorState &state, float x, float y) const;
  void select(EditorState &state) const;
  bool operator==(const PickerItem &other) const;
};

