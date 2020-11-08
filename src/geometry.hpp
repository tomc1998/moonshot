#pragma once
#include <algorithm>
#include <raylib.h>

enum class TripletOrientation { Colinear, Clockwise, Anticlockwise };

inline TripletOrientation
triplet_orientation(const Vector2 &p, const Vector2 &q, const Vector2 &r) {
  // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
  // for details of below formula.
  int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

  return (val == 0) ? TripletOrientation::Colinear
                    : ((val > 0) ? TripletOrientation::Clockwise
                                 : TripletOrientation::Anticlockwise);
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
inline bool is_colinear_point_on_segment(const Vector2 &p, const Vector2 &q,
                                         const Vector2 &r) {
  return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
          q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

// returns true if line segment 'p1q1' and 'p2q2' intersect.
// Pinched from
// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
inline bool line_segments_intersect(const Vector2 &p1, const Vector2 &q1,
                                    const Vector2 &p2, const Vector2 &q2) {
  // Find the four orientations needed for general and
  // special cases
  auto o1 = triplet_orientation(p1, q1, p2);
  auto o2 = triplet_orientation(p1, q1, q2);
  auto o3 = triplet_orientation(p2, q2, p1);
  auto o4 = triplet_orientation(p2, q2, q1);

  // General case
  if (o1 != o2 && o3 != o4)
    return true;

  // Special Cases
  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == TripletOrientation::Colinear &&
      is_colinear_point_on_segment(p1, p2, q1))
    return true;

  // p1, q1 and q2 are colinear and q2 lies on segment p1q1
  if (o2 == TripletOrientation::Colinear &&
      is_colinear_point_on_segment(p1, q2, q1))
    return true;

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == TripletOrientation::Colinear &&
      is_colinear_point_on_segment(p2, p1, q2))
    return true;

  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == TripletOrientation::Colinear &&
      is_colinear_point_on_segment(p2, q1, q2))
    return true;

  return false; // Doesn't fall in any of the above cases
}

inline double determinant2(double a, double b, double c, double d) {
  return a * d - b * c;
}

inline Vector2 get_line_intersection(const Vector2 &p1, const Vector2 &q1,
                                     const Vector2 &p2, const Vector2 &q2) {
  // line1 as a1x + b1y = c1
  auto a1 = q1.y - p1.y;
  auto b1 = p1.x - q1.x;
  auto c1 = a1 * p1.x + b1 * p1.y;

  // line2 as a2x + b2y = c2
  auto a2 = q2.y - p2.y;
  auto b2 = p2.x - q2.x;
  auto c2 = a2 * p2.x + b2 * p2.y;

  auto determinant = a1 * b2 - a2 * b1;

  Vector2 intersection{std::numeric_limits<float>::infinity(),
                       std::numeric_limits<float>::infinity()};

  if (determinant != 0) {
    intersection.x = (b2 * c1 - b1 * c2) / determinant;
    intersection.y = (a1 * c2 - a2 * c1) / determinant;
  }

  return intersection;
}