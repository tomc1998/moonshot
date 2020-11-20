#include "editor_state.hpp"
#include <algorithm>
#include <climits>

Level EditorState::serialise() {
  // Figure out size & offset of the tilemap by finding the extents of the tile
  // positions
  int min_x = INT_MAX, max_x = INT_MIN;
  int min_y = INT_MAX, max_y = INT_MIN;
  for (const auto &v : tiles) {
    min_x = std::min(v.x, min_x);
    max_x = std::max(v.x, max_x);
    min_y = std::min(v.y, min_y);
    max_y = std::max(v.y, max_y);
  }
  int top = min_y;
  int left = min_x;
  int w = std::max(0, 1 + max_x - min_x);
  int h = std::max(0, 1 + max_y - min_y);

  // Now create the tilemap. Tiles are row-major.
  std::vector<TileId> tilemap_tiles(w * h, 0);
  for (const auto &v : tiles) {
    tilemap_tiles[(v.y - top) * w + (v.x - left)] = v.val;
  }

  Level ret{tileset,
            {0, 0, w, h, tilemap_tiles},
            {
                Entity(EK_PLAYER, 16, 16),
            },
            {24, 24}};
  return ret;
}
