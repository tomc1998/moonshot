#include "geometry.hpp"
#include "nearest_intersector_finder.hpp"
#include <raymath.h>

NearestIntersectorFinder::NearestIntersectorFinder(
    const Vector2 &line_segment_start, const Vector2 &line_segment_end,
    float furthest_intersection_distance)
    : _line_segment_start(line_segment_start),
      _line_segment_end(line_segment_end), intersection_found(false),
      closest_distance_so_far(furthest_intersection_distance) {}

bool NearestIntersectorFinder::TestForNearestIntersection(
    const Vector2 &left, const Vector2 &right) {

  bool intersects_and_is_nearest = false;
  if (line_segments_intersect(_line_segment_start, _line_segment_end, left,
                              right)) {
    intersection_found = true;
    auto intersection = get_line_intersection(_line_segment_start,
                                              _line_segment_end, left, right);
    auto distance_to_intersection =
        Vector2Distance(_line_segment_start, intersection);

    if (distance_to_intersection < closest_distance_so_far) {
      closest_distance_so_far = distance_to_intersection;
      closest_intersection = intersection;
      intersects_and_is_nearest = true;
    }
  }

  return intersects_and_is_nearest;
}
