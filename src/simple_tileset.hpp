#pragma once

#include "tileset.hpp"
#include <memory>

std::shared_ptr<Tileset> tileset = nullptr;

/** Load the simple tileset */
inline std::shared_ptr<Tileset> load_simple_tileset() {
  if (tileset) {
    return tileset;
  }
  tileset =
      std::make_shared<Tileset>(LoadTexture("assets/simple_tileset.png"), 16);
  tileset->tiles.push_back({0, 0, false, false}); // Air
  tileset->tiles.push_back({0, 1, true});         // Brick
  tileset->tiles.push_back({1, 0, false});        // Floor
  return tileset;
}
