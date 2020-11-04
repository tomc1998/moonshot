#include <raylib.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "entity.hpp"
#include "entity_storage.hpp"
#include "simple_tileset.hpp"
#include "smooth_camera.hpp"
#include "tilemap.hpp"

constexpr float FPS = 144.0f;
constexpr float DT = 1.0f / FPS;

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

void update_entities(EntityStorage &es, float dt) {
  es.iter([dt](Entity &e) {
    e.pos.x += e.vel.x * dt;
    e.pos.y += e.vel.y * dt;
  });
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

void render_entities(const EntityStorage &es) {
  es.iter([](const Entity &e) { DrawRectangleV(e.pos, {16, 16}, WHITE); });
}

int main(void) {
  int screen_w = 1600;
  int screen_h = 900;
  InitWindow(screen_w, screen_h, "window_me");
  SetTargetFPS((int)FPS);

  // Disable raylib exiting when <esc> is pressed
  SetExitKey(0);

  SmoothCamera camera(screen_w, screen_h, 0, 0, 2, 0.05);

  const auto simple_tileset = load_simple_tileset();
  // clang-format off
  Tilemap tm{0, 0, 16, 16, {
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

  EntityStorage es;
  auto player = es.alloc(Entity(EK_PLAYER, 15, 15));

  while (!WindowShouldClose()) {
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

  CloseWindow();
  return 0;
}

