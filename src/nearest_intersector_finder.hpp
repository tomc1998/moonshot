#pragma once

#include <raylib.h>

class NearestIntersectorFinder {

public:
  bool intersection_found;
  float closest_distance_so_far;
  Vector2 closest_intersection;

  NearestIntersectorFinder(const Vector2 &line_segment_start,
                           const Vector2 &line_segment_end,
                           float furthest_intersection_distance);

  // return true if intersects and is the nearest so far
  bool TestForNearestIntersection(const Vector2 &left, const Vector2 &right);

private:
  const Vector2 &_line_segment_start;
  const Vector2 &_line_segment_end;
};
