#pragma once

#include "tileset.hpp"
#include <memory>

/** Load the simple tileset */
inline std::shared_ptr<Tileset> load_simple_tileset() {
  std::shared_ptr<Tileset> ret =
      std::make_shared<Tileset>(LoadTexture("assets/simple_tileset.png"), 16);
  ret->tiles.push_back({0, 0, false, false}); // Air
  ret->tiles.push_back({0, 1, true});         // Brick
  ret->tiles.push_back({1, 0, false});        // Floor
  return ret;
}
