#pragma once

#include <memory>

#include "sparse_matrix.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

struct EditorState {
  SparseMatrix<TileId> tiles;
  std::shared_ptr<Tileset> tileset;
  /** Boolean which is set when the mouse is pressed on the tilemap: if true,
   * delete tiles under the mouse; otherwise, add tiles under the mouse. */
  bool erasing;
};
