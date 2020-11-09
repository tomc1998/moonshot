#pragma once

#include <raymath.h>

inline Vector2 screen_to_world(const Camera2D &camera, Vector2 pos) {
  return Vector2Add(
      Vector2Scale(Vector2Subtract(pos, camera.offset), 1.0 / camera.zoom),
      camera.target);
}
