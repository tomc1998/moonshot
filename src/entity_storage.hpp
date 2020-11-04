#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

#include "entity.hpp"

using EntityId = uint64_t;

struct EntityStorage {
  std::vector<Entity> entity_list;
  /** Same length as entity_list, true indicates an entity is present, false is
   * an empty value */
  std::vector<bool> present_list;

  /** Iterate through the entities, call `f` on each present entity, f is
   * provided with an Entity. */
  template <typename F>
  inline void iter(F f) {
    assert(entity_list.size() == present_list.size());
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (present_list[ii]) {
        f(entity_list[ii]);
      }
    }
  }

  /** Const version of iter */
  template <typename F>
  inline void iter(F f) const {
    assert(entity_list.size() == present_list.size());
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (present_list[ii]) {
        f(entity_list[ii]);
      }
    }
  }

  /** Allocate a new entity, O(n)
   *
   * TODO Can optimise better than O(n)? Emplace rather than push_back + copy?
   */
  inline EntityId alloc(Entity e) {
    assert(entity_list.size() == present_list.size());
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (!present_list[ii]) {
        present_list[ii] = true;
        return ii;
      }
    }
    entity_list.push_back(e);
    present_list.push_back(true);
    return entity_list.size() - 1;
  }

  /** Returns null if not present */
  inline Entity* lookup(EntityId id) {
    assert(entity_list.size() == present_list.size());
    if (id >= entity_list.size() or not present_list[id]) {
      return nullptr;
    }
    return &entity_list[id];
  }

  /** Returns null if not present */
  inline const Entity* lookup(EntityId id) const {
    assert(entity_list.size() == present_list.size());
    if (id >= entity_list.size() or not present_list[id]) {
      return nullptr;
    }
    return &entity_list[id];
  }
};
