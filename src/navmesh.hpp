#pragma once

#include "tilemap.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <list>

using boost::adjacency_list;
using boost::edge_weight_t;
using boost::listS;
using boost::no_property;
using boost::property;
using boost::property_map;
using boost::undirectedS;
using boost::vecS;
using std::list;
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
  // position in the tiles vector. So an edge in the NavMesh is defined as two
  // cells in the tilemap.
  typedef std::pair<int, int> edge;

  // visitor clas that terminates when we find the goal
  class astar_goal_visitor : public boost::default_astar_visitor {
  public:
    astar_goal_visitor(vertex goal);
    void examine_vertex(vertex u, const navmesh_t &g);

  private:
    vertex m_goal;
  };

public:
  // Constructs a NavMesh based on a given Tilemap
  NavMesh(const Tilemap &tm, const Tileset &ts);

  // Returns a list of tile indexes that represent the path
  list<int> find_shortest_path(vertex start, vertex goal) const;

  // Returns a list of tile indexes that represent the path
  list<int> find_shortest_path(Tilecoords start, Tilecoords goal) const;

  const Tilemap &tm;
  const Tileset &ts;

private:
  void find_edges();
  void check_if_tiles_connected(
      int from_tile_index, int to_tile_index,
      vector<int> other_tile_indexes_which_must_not_collide,
      WeightMap &weightmap, cost weight);

  navmesh_t _navmesh;
};
