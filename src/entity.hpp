#pragma once

#include <raylib.h>

/** A tag to indicate what 'kind' of entity this is - found on every entity*/
enum EntityKind { EK_PLAYER, EK_ENEMY };

struct Entity {
  Vector2 pos, vel = {0, 0};
  EntityKind kind;
  // Bonus data, dependent on the value of `kind`.
  union {};

  inline Entity(EntityKind kind, float x, float y) : kind(kind), pos{x, y} {};
  inline Vector2 size() {
    switch (kind) {
    case EK_PLAYER:
    case EK_ENEMY:
      return {16, 16};
    }
  }
};
