#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include "game_state.hpp"
#include "globals.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include <memory>
#include <vector>

/** Stores a level in memory, which can be loaded into the game_state. Can be
 * serialised / deserialised to be stored on disk. */
struct Level {
  /** Name of the tileset */
  std::shared_ptr<Tileset> tileset;
  Tilemap tilemap;
  std::vector<Entity> entity_list;
  Vector2 laser_position;
  /** Initial camera position */
  Vector2 camera = {0.0, 0.0};
  float camera_zoom = 1.0;

  inline void load(GameState &state) const {
    state.es.clear();
    state.es.load_all(entity_list.begin(), entity_list.end());
    state.tm = tilemap;
    state.tileset = tileset;
    state.camera = SmoothCamera(screen_w, screen_h, camera.x, camera.y,
                                camera_zoom, CAMERA_SNAP_SPEED);
    state.laser.position = laser_position;
    // Find the player & set it
    state.player =
        state.es.find_if([](auto &e) { return e.kind == EK_PLAYER; });
  }
};

