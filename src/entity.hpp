#pragma once

#include <raylib.h>

/** A tag to indicate what 'kind' of entity this is - found on every entity*/
enum EntityKind { EK_PLAYER, EK_ENEMY, EK_MIRROR };

/** Data for EK_MIRROR entities */
struct MirrorData {
  /** Rotation of mirror, in radians */
  float rot;
};

struct Entity {
  Vector2 pos, vel = {0, 0};
  EntityKind kind;
  // Bonus data, dependent on the value of `kind`.
  union {
    MirrorData mirror;
  };

  inline Entity(EntityKind kind, float x, float y) : kind(kind), pos{x, y} {};
  inline Entity(EntityKind kind, float x, float y, MirrorData mirror)
      : kind(kind), pos{x, y}, mirror(mirror){};

  inline Vector2 size() {
    switch (kind) {
    case EK_PLAYER:
    case EK_ENEMY:
      return {16, 16};
    }
  }
};
