#pragma once

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
  Tileset simple_tileset;
  Tilemap tm;
  GameState() : camera(screen_w, screen_h, 0, 0, 1, 0.05) {}

  /** Load the level */
  void load() {
    player = es.alloc(Entity(EK_PLAYER, 16, 16));
    es.alloc(Entity(EK_MIRROR, 16, 64, MirrorData{PI / 4}));
    es.alloc(Entity(EK_MIRROR, 64, 64, MirrorData{-PI / 4}));
    es.alloc(Entity(EK_MIRROR, 64, 16, MirrorData{-PI / 4}));
    es.alloc(Entity(EK_MIRROR, 128, 16, MirrorData{PI / 3}));
    laser.position = Vector2{24, 24};
    simple_tileset = load_simple_tileset();
    // clang-format off
    tm = Tilemap{0, 0, 16, 16, {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        }};
    // clang-format on
  }
};
