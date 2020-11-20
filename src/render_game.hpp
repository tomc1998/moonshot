#pragma once

#include "constants.hpp"
#include "entity_storage.hpp"
#include "laser.hpp"
#include "mirror.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>

inline void render_entities(const EntityStorage &es) {
  es.iter([](const Entity &e) {
    if (e.kind == EK_MIRROR) {
      DrawLineEx(get_mirror_left(e), get_mirror_right(e), 4, WHITE);
    } else {
      DrawRectangleV(e.pos, {16, 16}, WHITE);
    }
  });
}

inline void render_laser_base(Vector2 pos) {
  DrawCircle(pos.x, pos.y, 5, GREEN);
}

inline void render_laser(const Laser &laser) {
  render_laser_base(laser.position);
  const Vector2 *current_vertex = &laser.position;
  for (const Vector2 &vertex : laser.vertices()) {
    DrawLineEx(*current_vertex, vertex, 2, GREEN);
    current_vertex = &vertex;
  }
}
