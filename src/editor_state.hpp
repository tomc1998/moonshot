#pragma once

#include <memory>

#include "level.hpp"
#include "picker_item.hpp"
#include "sparse_matrix.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

struct EditorState {
  SparseMatrix<TileId> tiles;
  std::shared_ptr<Tileset> tileset;
  /** If true, the mouse is down & we're drawing tiles on the canvas. This
   * isn't set if the mouse is pressed on top of the picker, for example. */
  bool drawing = false;
  /** Boolean which is set when drawing: if true,
   * delete tiles under the mouse; otherwise, add tiles under the mouse.
   * Undefined if drawing == false. */
  bool erasing = false;
  /** The current selected picker item */
  PickerItem picker_item = PickerItem((TileId)0);

  /** Serialise this editor state into a level, and return it */
  Level serialise();
  Vector2 laser_pos{8, 8};
};
