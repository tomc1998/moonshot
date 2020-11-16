#pragma once

#include <cassert>
#include <raylib.h>

/** A tag to indicate what 'kind' of entity this is - found on every entity*/
enum EntityKind { EK_PLAYER, EK_MIRROR, EK_ENEMY_BASIC };

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

struct Entity {
  Vector2 pos, vel = {0, 0};
  EntityKind kind;
  // Bonus data, dependent on the value of `kind`.
  union {
    MirrorData mirror;
    /** Enemy AI data, freed by entity_storage (since we can't have RAII types
     * in a union) */
    EnemyBasicData *enemy_basic;
  };

  inline Entity(EntityKind kind, float x, float y) : kind(kind), pos{x, y} {};
  inline Entity(EntityKind kind, float x, float y, MirrorData mirror)
      : kind(kind), pos{x, y}, mirror(mirror){};
  inline Entity(EntityKind kind, float x, float y, EnemyBasicData *enemy_basic)
      : kind(kind), pos{x, y}, enemy_basic(enemy_basic){};

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
