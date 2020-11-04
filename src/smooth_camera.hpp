#pragma once

#include <raylib.h>
#include <raymath.h>

/** A camera which gradually pans towards its target. Wraps a raylib camera,
 * maintains extra state for the intended target. */
struct SmoothCamera {
  Camera2D inner;
  float snap_speed;
  float zoom;
  Vector2 target;
  SmoothCamera(float screen_w, float screen_h, float x, float y, float zoom,
               float snap_speed)
      : snap_speed(snap_speed), zoom(zoom), target({x, y}) {
    inner.offset = (Vector2){(float)screen_w / 2, (float)screen_h / 2};
    inner.rotation = 0.0f;
    inner.target = target;
    inner.zoom = zoom;
  }
  void update() {
    inner.target = Vector2Add(
        inner.target,
        Vector2Scale(Vector2Subtract(target, inner.target), snap_speed));
    inner.zoom = inner.zoom + (zoom - inner.zoom) * snap_speed;
  }
};

