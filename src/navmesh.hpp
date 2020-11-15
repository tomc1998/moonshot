#pragma once

#include "tilemap.hpp"
#include <boost/graph/adjacency_list.hpp>

using boost::adjacency_list;
using boost::edge_weight_t;
using boost::listS;
using boost::no_property;
using boost::property;
using boost::property_map;
using boost::undirectedS;
using boost::vecS;
using std::vector;

// See https://www.boost.org/doc/libs/1_74_0/libs/graph/example/astar-cities.cpp
// for an example from which this class was based.

class NavMesh {

  typedef float cost;
  typedef adjacency_list<listS, vecS, undirectedS, no_property,
                         property<edge_weight_t, cost>>
      navmesh_t;

  typedef property_map<navmesh_t, edge_weight_t>::type WeightMap;
  typedef navmesh_t::vertex_descriptor vertex;
  typedef navmesh_t::edge_descriptor edge_descriptor;

  // Each cell in the tilemap can be specified as an integer which is its
  // position in the tiles vector. So an edge in the NavMesg is defined as two
  // cells in the tilemap.
  typedef std::pair<int, int> edge;

public:
  // Constructs a NavMesh based on a given Tilemap
  NavMesh(const Tilemap &tm, const Tileset &ts);

  void find_path(vertex start, vertex goal);

private:
  // Populates _edges
  void find_edges(const Tilemap &tm, const Tileset &ts);

  void check_if_tiles_connected(const Tilemap &tm, const Tileset &ts,
                                int from_tile_index, int to_tile_index);

  // The edges in our NavMesh define which cells in the tilemap can be travelled
  // between in one step.
  vector<edge> _edges;

  // The weight of an edge defines the cost to use that edge. If some tiles were
  // concrete and others were grass, we could make it less desireable to walk on
  // grass.
  vector<cost> _weights;

  navmesh_t _navmesh;
};
