#pragma once

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <vector>

#include "entity.hpp"

using EntityId = uint64_t;

struct EntityStorage {
  std::vector<Entity> entity_list;
  /** Same length as entity_list, true indicates an entity is present, false is
   * an empty value */
  std::vector<bool> present_list;

  /** Similar to `iter`, but passes the ID of the entity with the call, useful
   * if you want to do stuff like erase entities mid-loop.
   * Calls `f(id, entity)`*/
  template <typename F> inline void iter_id(F f) {
    assert(entity_list.size() == present_list.size());
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (present_list[ii]) {
        f(ii, entity_list[ii]);
      }
    }
  }

  /** Const version of iter_id */
  template <typename F> inline void iter_id(F f) const {
    assert(entity_list.size() == present_list.size());
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (present_list[ii]) {
        f(ii, entity_list[ii]);
      }
    }
  }

  /** Iterate through the entities, call `f` on each present entity, f is
   * provided with an Entity. */
  template <typename F> inline void iter(F f) {
    assert(entity_list.size() == present_list.size());
    iter_id([f](auto id, auto &e) { f(e); });
  }

  /** Const version of iter */
  template <typename F> inline void iter(F f) const {
    assert(entity_list.size() == present_list.size());
    iter_id([f](auto id, const auto &e) { f(e); });
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

  /** Delete an entity, also performs any additional memory freeing. NOP on
   * double-free.
   * It *IS* safe to call this whilst iterating through entities via
   * iter/iter_id, because this won't actually move entities, simply marks
   * slots as available for new entities when allocating. */
  inline void erase(EntityId id) {
    assert(entity_list.size() == present_list.size());
    if (id >= entity_list.size() or not present_list[id]) {
      printf("ERROR: Double free entity ID %ld\n", id);
    }
    present_list[id] = false;
  }

  /** Returns null if not present */
  inline Entity *lookup(EntityId id) {
    assert(entity_list.size() == present_list.size());
    if (id >= entity_list.size() or not present_list[id]) {
      return nullptr;
    }
    return &entity_list[id];
  }

  /** Returns null if not present */
  inline const Entity *lookup(EntityId id) const {
    assert(entity_list.size() == present_list.size());
    if (id >= entity_list.size() or not present_list[id]) {
      return nullptr;
    }
    return &entity_list[id];
  }

  /** Clear all entities - useful for reloading the game state. */
  inline void clear() {
    entity_list.clear();
    present_list.clear();
  }

  /** Load a list of entities. Iterators should support - operator to s */
  template <typename Iter> inline void load_all(Iter start, Iter end) {
    entity_list.insert(entity_list.end(), start, end);
    std::fill_n(std::back_inserter(present_list), std::distance(start, end),
                true);
  }

  /** Returns -1 if not found */
  template <typename UnaryPredicate>
  inline EntityId find_if(UnaryPredicate pred) {
    for (int ii = 0; ii < entity_list.size(); ++ii) {
      if (present_list[ii] and pred(entity_list[ii])) {
        return ii;
      }
    }
    return -1;
  }
};
