#pragma once

#include "entity.hpp"
#include "geometry.hpp"
#include "mirror.hpp"
#include <raylib.h>
#include <raymath.h>

inline Vector2 getLaserRear(const Entity &e) {
  assert(e.kind == EK_LASER);
  return Vector2Subtract(e.pos,
                         Vector2Scale(Vector2Normalize(e.vel), LASER_LEN));
}

inline Vector2 getLaserFront(const Entity &e) {
  assert(e.kind == EK_LASER);
  return e.pos;
}

inline bool laserIntersectsMirror(const Entity &laser, const Entity &mirror) {
  assert(laser.kind == EK_LASER);
  assert(mirror.kind == EK_MIRROR);

  return lineSegmentsIntersect(getLaserFront(laser), getLaserRear(laser),
                               getMirrorLeft(mirror), getMirrorRight(mirror));
}

inline Vector2 getLaserMirrorIntersection(const Entity &laser,
                                          const Entity &mirror) {
  assert(laser.kind == EK_LASER);
  assert(mirror.kind == EK_MIRROR);
}