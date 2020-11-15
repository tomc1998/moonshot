#include "laser.hpp"
#include "constants.hpp"
#include "game_state.hpp"
#include "geometry.hpp"
#include "mirror.hpp"
#include "nearest_intersector_finder.hpp"
#include <algorithm>

Laser::Laser()
    : max_length(MIN_LASER_LENGTH), position(Vector2{0, 0}), length(0),
      firing_angle(0) {}

void Laser::update(GameState &state, float dt) {
  if (IsMouseButtonDown(0)) {
    if (length == 0) {
      // Set the firing angle from towards the mouse
      const auto mouse_position_relative_to_player = Vector2Subtract(
          GetMousePosition(), {(float)screen_w / 2, (float)screen_h / 2});
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

    // Check collision with enemies
    state.es.iter_id([&](EntityId id, const auto &e) {
      switch (e.kind) {
      case EK_ENEMY_BASIC:
        for (int ii = -1; ii < (int)this->_vertices.size() - 1; ++ii) {
          const auto p0 = (ii == -1) ? this->position : this->_vertices[ii];
          const auto q0 = this->_vertices[ii + 1];
          const auto size = e.size();
          Vector2 e_lines[] = {{e.pos.x, e.pos.y},
                               {e.pos.x + size.x, e.pos.y},
                               {e.pos.x + size.x, e.pos.y + size.y},
                               {e.pos.x, e.pos.y + size.y}};
          constexpr int nlines = sizeof(e_lines) / sizeof(e_lines[0]);
          bool intersects = false;
          for (int jj = 0; jj < nlines; ++jj) {
            const auto p1 = e_lines[jj];
            const auto q1 = e_lines[(jj + 1) % nlines];
            if (line_segments_intersect(p0, q0, p1, q1)) {
              intersects = true;
              break;
            }
          }
          if (intersects) {
            state.es.erase(id);
          }
        }
        break;
      default:
        break;
      }
    });
  } else {
    length = 0;
  }
}

void Laser::calculate_laser_vertices(const GameState &state) {

  _vertices.clear();

  // Trace the path of the laser forwards, turning at mirrors, or terminating
  // at walls.
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

    // Now find all entities that intersect with that line segment. We want
    // the entity that is closest to the current position.
    NearestIntersectorFinder nearestIntersectionFinder(
        current_position, line_segment_end,
        std::numeric_limits<float>::infinity());

    const Entity *closest_intersecting_entity = nullptr;
    state.es.iter([&](const Entity &e) {
      if (&e != previous_intersecting_entity) {
        switch (e.kind) {
        case EK_MIRROR: {
          auto mirror_left = get_mirror_left(e);
          auto mirror_right = get_mirror_right(e);
          if (nearestIntersectionFinder.TestForNearestIntersection(
                  mirror_left, mirror_right)) {
            closest_intersecting_entity = &e;
          }
        } break;
        default:
          // laser goes straight through all other entity types
          break;
        }
      }
    });

    // TODO: We also need to check for intersections with wall tiles.
    const Tile *closest_intersecting_tile = nullptr;
    const Tileset &ts = *state.tileset;
    const Tilemap &tm = state.tm;
    for (int xx = 0; xx < tm.w; ++xx) {
      for (int yy = 0; yy < tm.h; ++yy) {
        const auto &tile = ts.tiles[tm.tiles[yy * tm.w + xx]];
        if (!tile.collides) {
          // If a tile doesn't collide, we'll also say that it doesn't stop
          // the laser. We might want to later add an "is_transparent"
          // property to a Tile so we can have colliding glass tile that the
          // laser still passes through.
          continue;
        }

        // Get the tile corner points in clockwise order around the tile
        const Vector2 tile_corner_1{tm.x + xx * ts.tile_size,
                                    tm.y + yy * ts.tile_size};
        const Vector2 tile_corner_2{tile_corner_1.x + ts.tile_size,
                                    tile_corner_1.y};
        const Vector2 tile_corner_3{tile_corner_1.x + ts.tile_size,
                                    tile_corner_1.y + ts.tile_size};
        const Vector2 tile_corner_4{tile_corner_1.x,
                                    tile_corner_1.y + ts.tile_size};

        // A tile has four sides, each side being a line segment, so check for
        // intersection with any of those four sides. If any do, take the
        // closest one, and then see if it is closer than the closest
        // intersecting (mirror) entity

        if (nearestIntersectionFinder.TestForNearestIntersection(
                tile_corner_1, tile_corner_2)) {
          closest_intersecting_tile = &tile;
          closest_intersecting_entity = nullptr;
        }

        if (nearestIntersectionFinder.TestForNearestIntersection(
                tile_corner_2, tile_corner_3)) {
          closest_intersecting_tile = &tile;
          closest_intersecting_entity = nullptr;
        }

        if (nearestIntersectionFinder.TestForNearestIntersection(
                tile_corner_3, tile_corner_4)) {
          closest_intersecting_tile = &tile;
          closest_intersecting_entity = nullptr;
        }

        if (nearestIntersectionFinder.TestForNearestIntersection(
                tile_corner_4, tile_corner_1)) {
          closest_intersecting_tile = &tile;
          closest_intersecting_entity = nullptr;
        }
      }
    }

    // Now decide what to do at the intersecting entity
    if (closest_intersecting_entity != nullptr) {

      // Keep track of this intersecting entity so we don't immediately
      // intersect with it again
      previous_intersecting_entity = closest_intersecting_entity;

      switch (closest_intersecting_entity->kind) {
      case EK_MIRROR: {
        _vertices.emplace_back(nearestIntersectionFinder.closest_intersection);
        remaining_length -= nearestIntersectionFinder.closest_distance_so_far;

        auto approachingVector = Vector2Subtract(
            nearestIntersectionFinder.closest_intersection, current_position);
        current_position = nearestIntersectionFinder.closest_intersection;

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
    } else if (closest_intersecting_tile != nullptr) {
      // We've hit a tile, so stop the laser at that point
      _vertices.emplace_back(nearestIntersectionFinder.closest_intersection);
      remaining_length = 0;
    } else {
      // There are no intersecting entities, so just continue the laser for
      // the remaining length
      _vertices.emplace_back(line_segment_end);
      remaining_length = 0;
    }
  }
}
