#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <variant>

// Mirror positioned at the top left.

inline Vector2 get_mirror_left(const Entity &e) {
  assert(e.kind == EK_MIRROR);
  Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
  return Vector2{
      c.x - cosf(std::get<MirrorData>(e.data).rot) * MIRROR_WIDTH / 2,
      c.y - sinf(std::get<MirrorData>(e.data).rot) * MIRROR_WIDTH / 2};
}

inline Vector2 get_mirror_right(const Entity &e) {
  assert(e.kind == EK_MIRROR);
  Vector2 c{e.pos.x + MIRROR_WIDTH / 2, e.pos.y + MIRROR_WIDTH / 2};
  return Vector2{
      c.x + cosf(std::get<MirrorData>(e.data).rot) * MIRROR_WIDTH / 2,
      c.y + sinf(std::get<MirrorData>(e.data).rot) * MIRROR_WIDTH / 2};
}
