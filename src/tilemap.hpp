#pragma once

#include <cassert>
#include <cstdint>

#include "tileset.hpp"

using TileId = uint16_t;

struct Tilemap {
  float x, y;
  int w, h;
  /** size == w * h */
  std::vector<TileId> tiles;

  /** Draw the tilemap with the given tileset at the given offset */
  inline void render(const Tileset &ts) const {
    assert(w * h == tiles.size());
    for (int xx = 0; xx < w; ++xx) {
      for (int yy = 0; yy < h; ++yy) {
        const auto &tile = ts.tiles[tiles[yy * w + xx]];
        if (!tile.visible) {
          continue;
        }
        const Rectangle tile_rect{(float)tile.x * ts.tile_size,
                                  (float)tile.y * ts.tile_size,
                                  (float)ts.tile_size, (float)ts.tile_size};
        DrawTextureRec(ts.tex, tile_rect,
                       {x + xx * ts.tile_size, y + yy * ts.tile_size}, WHITE);
      }
    }
  }
};
