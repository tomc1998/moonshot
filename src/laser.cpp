#include "constants.hpp"
#include "game_state.hpp"
#include "geometry.hpp"
#include "laser.hpp"
#include "mirror.hpp"
#include <algorithm>

Laser::Laser()
    : max_length(MIN_LASER_LENGTH), position(Vector2{0, 0}), length(0),
      firing_angle(0) {}

void Laser::update(const GameState &state, float dt) {

  if (IsMouseButtonDown(0)) {
    if (length == 0) {
      // Set the firing angle from towards the mouse
      const auto mouse_position_relative_to_player =
          Vector2Subtract(GetMousePosition(), {screen_w / 2, screen_h / 2});
      const auto player_position =
          Vector2Add(state.es.lookup(state.player)->pos, Vector2{8, 8});
      const auto mouse_position =
          Vector2Add(mouse_position_relative_to_player, player_position);
      const auto delta_to_mouse = Vector2Subtract(mouse_position, position);
      firing_angle = atan2f(-delta_to_mouse.y, delta_to_mouse.x);
    }

    if (length < max_length) {
      // advance the laser by increasing its length
      length = Clamp(length + LASER_SPEED * dt, 0, max_length);
    }
  } else {
    length = 0;
  }
}

void Laser::calculate_laser_vertices(const GameState &state) {

  _vertices.clear();

  // Trace the path of the laser forwards, turning at mirrors, or terminating at
  // walls.
  auto current_position = position;
  auto current_angle = firing_angle;
  auto remaining_length = length;
  const Entity *previous_intersecting_entity = nullptr;
  while (remaining_length > 0) {
    // Construct a line segement from the current position forwards for the
    // remaining length.
    auto line_segment_end =
        Vector2{current_position.x + remaining_length * cosf(current_angle),
                current_position.y - remaining_length * sinf(current_angle)};

    // Now find all entities that intersect with that line segment. We want the
    // entity that is closest to the current position.
    float closest_distance = std::numeric_limits<float>::infinity();
    Vector2 closest_intersection;
    const Entity *closest_intersecting_entity = nullptr;
    state.es.iter([&](const Entity &e) {
      if (&e != previous_intersecting_entity) {
        switch (e.kind) {
        case EK_MIRROR: {
          auto mirror_left = get_mirror_left(e);
          auto mirror_right = get_mirror_right(e);
          if (line_segments_intersect(current_position, line_segment_end,
                                      mirror_left, mirror_right)) {
            // The mirror intersects with the laser path, so find the
            // intersection point so we can see determine whether it is
            // closest.

            auto intersection = get_line_intersection(
                current_position, line_segment_end, mirror_left, mirror_right);
            auto distance_to_intersection =
                Vector2Distance(current_position, intersection);

            if (distance_to_intersection < closest_distance) {
              closest_distance = distance_to_intersection;
              closest_intersection = intersection;
              closest_intersecting_entity = &e;
            }
          }

        } break;
        default:
          // laser goes straight through all other entity types
          break;
        }
      }
    });

    // TODO: We also need to check for intersections with wall tiles.

    // Now decide what to do at the intersecting entity
    if (closest_intersecting_entity != nullptr) {

      // Keep track of this intersecting entity so we don't immediately
      // intersect with it again
      previous_intersecting_entity = closest_intersecting_entity;

      switch (closest_intersecting_entity->kind) {
      case EK_MIRROR: {
        _vertices.emplace_back(closest_intersection);
        remaining_length -= closest_distance;

        auto approachingVector =
            Vector2Subtract(closest_intersection, current_position);
        current_position = closest_intersection;

        // The mirror reflects so work out what the new angle of the laser is
        // from the intersection point
        auto mirrorVector =
            Vector2Subtract(get_mirror_right(*closest_intersecting_entity),
                            get_mirror_left(*closest_intersecting_entity));
        auto mirrorNormal = Vector2Normalize(Vector2Rotate(mirrorVector, -90));
        auto departingLaserVector =
            Vector2Reflect(approachingVector, mirrorNormal);
        current_angle = atan2f(-departingLaserVector.y, departingLaserVector.x);

      } break;
      default:
        assert(false); // TODO: handle other entity types
        break;
      }
    } else {
      // There are no intersecting entities, so just continue the laser for the
      // remaining length
      _vertices.emplace_back(line_segment_end);
      remaining_length = 0;
    }
  }
}
