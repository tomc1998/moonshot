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
      DrawLineEx(getMirrorLeft(e), getMirrorRight(e), 4, WHITE);
    } else if (e.kind == EK_LASER) {
      DrawLineEx(getLaserRear(e), getLaserFront(e), 2, RED);
    } else {
      DrawRectangleV(e.pos, {16, 16}, WHITE);
    }
  });
}
