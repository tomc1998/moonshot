#pragma once

#include "constants.hpp"
#include "entity_storage.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>

inline void render_entities(const EntityStorage &es) {
  es.iter([](const Entity &e) {
    if (e.kind == EK_MIRROR) {
      // Mirror positioned at the top left, find the center
      Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
      Vector2 start{c.x - cosf(e.mirror.rot) * MIRROR_WIDTH / 2,
                    c.y - sinf(e.mirror.rot) * MIRROR_WIDTH / 2};
      Vector2 end{c.x + cosf(e.mirror.rot) * MIRROR_WIDTH / 2,
                  c.y + sinf(e.mirror.rot) * MIRROR_WIDTH / 2};
      DrawLineEx(start, end, 4, WHITE);
    } else if (e.kind == EK_LASER) {
      Vector2 start = Vector2Subtract(
          e.pos, Vector2Scale(Vector2Normalize(e.vel), LASER_LEN));
      Vector2 end = e.pos;
      DrawLineEx(start, end, 2, WHITE);
    } else {
      DrawRectangleV(e.pos, {16, 16}, WHITE);
    }
  });
}

