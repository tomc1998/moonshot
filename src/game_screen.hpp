#pragma once

#include "constants.hpp"
#include "entity.hpp"
#include "entity_storage.hpp"
#include "game_screen.hpp"
#include "globals.hpp"
#include "render_game.hpp"
#include "screen.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"
#include "update_game.hpp"
#include <cmath>

/** Check for input, and move the player */
void move_player(EntityStorage &es, EntityId playerId) {
  auto player = es.lookup(playerId);
  assert(player);
  if (IsKeyDown(KEY_RIGHT) or IsKeyDown(KEY_D)) {
    player->vel.x = 100.0f;
  } else if (IsKeyDown(KEY_LEFT) or IsKeyDown(KEY_A)) {
    player->vel.x = -100.0f;
  } else {
    player->vel.x = 0;
  }
  if (IsKeyDown(KEY_UP) or IsKeyDown(KEY_W)) {
    player->vel.y = -100.0f;
  } else if (IsKeyDown(KEY_DOWN) or IsKeyDown(KEY_S)) {
    player->vel.y = 100.0f;
  } else {
    player->vel.y = 0;
  }
}

/** Just snaps positions, call after processing velocities */
void resolve_tile_collisions(EntityStorage &es, const Tileset &ts,
                             const Tilemap &tm) {
  es.iter([&](Entity &e) {
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

struct GameScreen : public Screen {
  EntityStorage es;
  EntityId player;
  Tileset simple_tileset;
  Tilemap tm;
  SmoothCamera camera;
  GameScreen() : camera(screen_w, screen_h, 0, 0, 2, 0.05) {}
  void on_mount() {
    player = es.alloc(Entity(EK_PLAYER, 16, 16));
    es.alloc(Entity(EK_MIRROR, 16, 64, MirrorData{PI / 4}));
    simple_tileset = load_simple_tileset();
    // clang-format off
    tm = Tilemap{0, 0, 16, 16, {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        }};
    // clang-format on
  }
  void on_frame() {
    // Update
    move_player(es, player);
    update_entities(es, DT);
    resolve_tile_collisions(es, simple_tileset, tm);

    camera.target =
        Vector2Add(es.lookup(player)->pos, {es.lookup(player)->size().x / 2,
                                            es.lookup(player)->size().y / 2});
    camera.update();

    // Draw
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera.inner);
    tm.render(simple_tileset);
    render_entities(es);
    EndMode2D();

    DrawFPS(10, screen_h - 26);
    EndDrawing();
  }
};
