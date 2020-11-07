#pragma once

#include "entity.hpp"
#include <raylib.h>
#include <raymath.h>

// Mirror positioned at the top left.

inline Vector2 getMirrorLeft(const Entity &e) {
  assert(e.kind == EK_MIRROR);
  Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
  return Vector2{c.x - cosf(e.mirror.rot) * MIRROR_WIDTH / 2,
                 c.y - sinf(e.mirror.rot) * MIRROR_WIDTH / 2};
}

inline Vector2 getMirrorRight(const Entity &e) {
  assert(e.kind == EK_MIRROR);
  Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
  return Vector2{c.x + cosf(e.mirror.rot) * MIRROR_WIDTH / 2,
                 c.y + sinf(e.mirror.rot) * MIRROR_WIDTH / 2};
}
