#pragma once

#include <raylib.h>
#include <vector>

class GameState;

class Laser {

public:
  Vector2 position;
  float length;       // length increases as laser advances
  float firing_angle; // in radians
  float max_length;

  Laser();

  inline const std::vector<Vector2> &vertices() const { return _vertices; }

  void calculate_laser_vertices(const GameState &state);

  void update(GameState &state, float dt);

private:
  std::vector<Vector2> _vertices;
};
