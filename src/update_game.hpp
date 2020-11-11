#pragma once

#include "entity.hpp"
#include "entity_storage.hpp"
#include "game_state.hpp"

inline void update_entities(GameState &state, float dt) {
  state.es.iter([dt](Entity &e) {
    e.pos.x += e.vel.x * dt;
    e.pos.y += e.vel.y * dt;
  });
}

/** Check for input, and update the player */
inline void update_player(GameState &state, EntityId playerId) {
  auto player = state.es.lookup(playerId);
  assert(player);

  // Movement
  if (IsKeyDown(KEY_RIGHT) or IsKeyDown(KEY_D)) {
    player->vel.x = PLAYER_SPEED;
  } else if (IsKeyDown(KEY_LEFT) or IsKeyDown(KEY_A)) {
    player->vel.x = -PLAYER_SPEED;
  } else {
    player->vel.x = 0;
  }
  if (IsKeyDown(KEY_UP) or IsKeyDown(KEY_W)) {
    player->vel.y = -PLAYER_SPEED;
  } else if (IsKeyDown(KEY_DOWN) or IsKeyDown(KEY_S)) {
    player->vel.y = PLAYER_SPEED;
  } else {
    player->vel.y = 0;
  }
}

/** Just snaps positions, call after processing velocities */
inline void resolve_tile_collisions(GameState &state) {
  const Tileset &ts = *state.tileset;
  const Tilemap &tm = state.tm;
  state.es.iter([&](Entity &e) {
    const auto size = e.size();
    for (int xx = 0; xx < tm.w; ++xx) {
      for (int yy = 0; yy < tm.h; ++yy) {
        const Rectangle e_rect{e.pos.x, e.pos.y, size.x, size.y};
        const auto &tile = ts.tiles[tm.tiles[yy * tm.w + xx]];
        if (!tile.collides) {
          continue;
        }
        const Rectangle tile_rect{tm.x + xx * ts.tile_size,
                                  tm.y + yy * ts.tile_size, (float)ts.tile_size,
                                  (float)ts.tile_size};
        auto rec = GetCollisionRec(tile_rect, e_rect);
        // Handle edge case where we collide with the corner of a tile in a
        // wall, and get 'stuck' on an otherwise smooth wall - ignore the
        // collision in this case, deal with it next frame. Go for a high
        // enough FPS that it won't matter.
        if (rec.width < 2 and rec.height < 2) {
          continue;
        }
        // Only resolve on the axis which requires the smallest resolution
        if (rec.width < rec.height) {
          if (e.pos.x > tile_rect.x) {
            e.pos.x += std::max(0.f, rec.width - 1);
          } else {
            e.pos.x -= std::max(0.f, rec.width - 1);
          }
        } else {
          if (e.pos.y > tile_rect.y) {
            e.pos.y += std::max(0.f, rec.height - 1);
          } else {
            e.pos.y -= std::max(0.f, rec.height - 1);
          }
        }
      }
    }
  });
}

inline void game_update(GameState &state) {
  update_player(state, state.player);
  update_entities(state, DT);
  state.laser.update(state, DT);
  state.laser.calculate_laser_vertices(state);
  resolve_tile_collisions(state);

  state.camera.target =
      Vector2Add(state.es.lookup(state.player)->pos,
                 {state.es.lookup(state.player)->size().x / 2,
                  state.es.lookup(state.player)->size().y / 2});
  state.camera.update();
}
