#pragma once

#include "navmesh.hpp"
#include "tilemap.hpp"
#include <cassert>
#include <raylib.h>
#include <variant>

/** A tag to indicate what 'kind' of entity this is - found on every entity*/
enum EntityKind { EK_PLAYER, EK_MIRROR, EK_ENEMY_BASIC };

inline const char *ek_name(EntityKind kind) {
  switch (kind) {
  case EK_PLAYER:
    return "Player";
  case EK_MIRROR:
    return "Mirror";
  case EK_ENEMY_BASIC:
    return "Enemy Basic";
  }
}

/** Calls `f` with each entity kind in succession */
template <typename F> inline void all_entity_kinds(F f) {
  // Do this with a switch, so that we're warned when a new entitykind is added
  for (const auto k : {EK_PLAYER, EK_MIRROR, EK_ENEMY_BASIC}) {
    switch (k) {
    case EK_PLAYER:
      f(k);
      break;
    case EK_MIRROR:
      f(k);
      break;
    case EK_ENEMY_BASIC:
      f(k);
      break;
    }
  }
}

/** Data for EK_MIRROR entities */
struct MirrorData {
  /** Rotation of mirror, in radians */
  float rot;
};

enum class EnemyBasicActionKind { MOVE, WAIT };
struct EnemyBasicAction {
  EnemyBasicActionKind kind;
  union {
    /** Point in map to move to, pathing handled separately */
    Vector2 move;
    /** Seconds to wait */
    float wait;
  };
  inline EnemyBasicAction(Vector2 move_)
      : kind(EnemyBasicActionKind::MOVE), move(move_) {}
  inline EnemyBasicAction(float wait_)
      : kind(EnemyBasicActionKind::WAIT), wait(wait_) {}
};

/** A basic enemy just processes a loop of 'actions', with no 'intelligence'
 * besides reacting to a player */
struct EnemyBasicData {
  std::vector<EnemyBasicAction> actions;
  /** Pointer into `actions` */
  int curr_action = 0;
  float time_in_action = 0.0;
  inline void next_action() {
    curr_action = (curr_action + 1) % actions.size();
    time_in_action = 0;
  }
};

struct EnemyPathAction {
  EnemyBasicActionKind kind;
  union {
    /** tile in map to move to */
    Tilecoords move;
    /** Seconds to wait */
    float wait;
  };
  inline EnemyPathAction(Tilecoords move_)
      : kind(EnemyBasicActionKind::MOVE), move(move_) {}
  inline EnemyPathAction(float wait_)
      : kind(EnemyBasicActionKind::WAIT), wait(wait_) {}
};

/** A sequence of path movements in a loop */
struct EnemyPathData {
  std::vector<EnemyPathAction> actions;
};

struct NoEnemyData {};

using EntityData = std::variant<NoEnemyData, MirrorData, EnemyBasicData>;

struct Entity {
  Vector2 pos, vel = {0, 0};
  EntityKind kind;
  EntityData data;

  inline Entity(EntityKind kind, float x, float y) : kind(kind), pos{x, y} {};
  inline Entity(EntityKind kind, float x, float y, MirrorData mirror)
      : kind(kind), pos{x, y}, data(mirror){};
  inline Entity(EntityKind kind, float x, float y, EnemyBasicData enemy_basic)
      : kind(kind), pos{x, y}, data(enemy_basic){};

  inline Entity(EntityKind kind, Tilecoords start_tile,
                EnemyPathData enemy_path, const NavMesh &navmesh)
      : kind(kind), pos{(float)(navmesh.ts.tile_size * start_tile.tile_x),
                        (float)(navmesh.ts.tile_size * start_tile.tile_y)} {
    // We need to convert the enemy path data into enemy basic data
    EnemyBasicData enemy_basic;
    Tilecoords prev_tile = start_tile;
    for (const EnemyPathAction &action : enemy_path.actions) {
      switch (action.kind) {
      case EnemyBasicActionKind::MOVE: {
        // Add an EnemyBasicAction MOVE for each step in the shortest path
        auto shortest_path = navmesh.find_shortest_path(prev_tile, action.move);
        for (auto tile_index : shortest_path) {
          auto tile_coords = navmesh.tm.tile_coords(tile_index);
          enemy_basic.actions.emplace_back(
              Vector2{(float)(navmesh.ts.tile_size * tile_coords.tile_x),
                      (float)(navmesh.ts.tile_size * tile_coords.tile_y)});
        }
        prev_tile = action.move;
      } break;
      case EnemyBasicActionKind::WAIT:
        // Add an EnemyBasicAction WAIT for the wait
        enemy_basic.actions.emplace_back(action.wait);
        break;
      }
    }
    this->data = enemy_basic;
  };

  inline Vector2 size() const {
    switch (kind) {
    case EK_PLAYER:
    case EK_ENEMY_BASIC:
    case EK_MIRROR:
      return {16, 16};
    }
    assert(false && "Switch fell through due to improper value of `kind`");
  }
};
