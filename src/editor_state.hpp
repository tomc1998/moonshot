#pragma once

#include <memory>

#include "level.hpp"
#include "sparse_matrix.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

struct EditorState {
  SparseMatrix<TileId> tiles;
  std::shared_ptr<Tileset> tileset;
  /** If true, the mouse is down & we're drawing tiles on the canvas. This
   * isn't set if the mouse is pressed on top of the picker, for example. */
  bool drawing_tiles = false;
  /** Boolean which is set when drawing tiles: if true,
   * delete tiles under the mouse; otherwise, add tiles under the mouse.
   * Undefined if drawing_tiles == false. */
  bool erasing = false;
  /** The current selected tile - an index into tileset->tiles. */
  int curr_tile = 1;

  /** Serialise this editor state into a level, and return it */
  Level serialise();
};
