#pragma once
#include <algorithm>
#include <raylib.h>

using std::max;
using std::min;

enum class TripletOrientation { Colinear, Clockwise, Anticlockwise };

TripletOrientation tripletOrientation(Vector2 p, Vector2 q, Vector2 r) {
  // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
  // for details of below formula.
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

  return (val == 0) ? TripletOrientation::Colinear
                    : ((val > 0) ? TripletOrientation::Clockwise
                                 : TripletOrientation::Anticlockwise);
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool isColinearPointOnSegment(Vector2 p, Vector2 q, Vector2 r) {
  return (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
          q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y));
}

// returns true if line segment 'p1q1' and 'p2q2' intersect.
// Pinched from
// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool lineSegmentsIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2) {
  // Find the four orientations needed for general and
  // special cases
  auto o1 = tripletOrientation(p1, q1, p2);
  auto o2 = tripletOrientation(p1, q1, q2);
  auto o3 = tripletOrientation(p2, q2, p1);
  auto o4 = tripletOrientation(p2, q2, q1);

  // General case
  if (o1 != o2 && o3 != o4)
    return true;

  // Special Cases
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == TripletOrientation::Colinear &&
      isColinearPointOnSegment(p1, p2, q1))
    return true;

  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == TripletOrientation::Colinear &&
      isColinearPointOnSegment(p1, q2, q1))
    return true;

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == TripletOrientation::Colinear &&
      isColinearPointOnSegment(p2, p1, q2))
    return true;

  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == TripletOrientation::Colinear &&
      isColinearPointOnSegment(p2, q1, q2))
    return true;

  return false; // Doesn't fall in any of the above cases
}