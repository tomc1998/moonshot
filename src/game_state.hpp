#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include "globals.hpp"
#include "laser.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"

struct GameState {
  SmoothCamera camera;
  EntityStorage es;
  EntityId player;
  Laser laser;
  std::shared_ptr<Tileset> tileset;
  Tilemap tm;
  GameState() : camera(screen_w, screen_h, 0, 0, 1, CAMERA_SNAP_SPEED) {}
};
