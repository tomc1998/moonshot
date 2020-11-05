#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include <raylib.h>

inline void render_entities(const EntityStorage &es) {
  es.iter([](const Entity &e) {
    if (e.kind == EK_MIRROR) {
      // Mirror positioned at the top left, find the center
      Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
      Vector2 start{c.x - cos(e.mirror.rot) * MIRROR_WIDTH / 2,
                    c.y - sin(e.mirror.rot) * MIRROR_WIDTH / 2};
      Vector2 end{c.x + cos(e.mirror.rot) * MIRROR_WIDTH / 2,
                  c.y + sin(e.mirror.rot) * MIRROR_WIDTH / 2};
      DrawLineEx(start, end, 4, WHITE);
    } else {
      DrawRectangleV(e.pos, {16, 16}, WHITE);
    }
  });
}

