#pragma once

#include <raylib.h>

#include <vector>

struct Tile {
  /** x/y tile offset into tileset texture. For a tile at pixel 16,16, with
   * tile size == 8, the tile would be at 2,2. */
  int x, y;
  bool collides;
  bool visible = true;
};

struct Tileset {
  Texture2D tex;
  /** Tile width / height in pixels */
  int tile_size;
  std::vector<Tile> tiles;
};
