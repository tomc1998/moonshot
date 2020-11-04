#pragma once

#include "tileset.hpp"

/** Load the simple tileset */
inline Tileset load_simple_tileset() {
  Tileset ret{LoadTexture("assets/simple_tileset.png"), 16};
  ret.tiles.push_back({0, 0, false, false}); // Air
  ret.tiles.push_back({0, 1, true});         // Brick
  ret.tiles.push_back({1, 0, false});        // Floor
  return ret;
}
