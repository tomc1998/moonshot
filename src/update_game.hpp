#pragma once

#include "entity.hpp"
#include "entity_storage.hpp"

void update_entities(EntityStorage &es, float dt) {
  es.iter([dt](Entity &e) {
    e.pos.x += e.vel.x * dt;
    e.pos.y += e.vel.y * dt;
  });
}

