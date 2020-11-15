#include "navmesh.hpp"
#include <boost/graph/astar_search.hpp>
#include <iostream>
#include <list>

using boost::astar_heuristic;
using boost::edge_weight;
using boost::graph_traits;
using boost::vertex_index;
using std::cout;
using std::endl;
using std::list;

// euclidean distance heuristic
template <class Graph, class CostType>
class distance_heuristic : public astar_heuristic<Graph, CostType> {
public:
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  distance_heuristic(Vertex goal) {}
  CostType operator()(Vertex u) { return 1; }
};

struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor {
public:
  astar_goal_visitor(Vertex goal) : m_goal(goal) {}
  template <class Graph> void examine_vertex(Vertex u, Graph &g) {
    if (u == m_goal)
      throw found_goal();
  }

private:
  Vertex m_goal;
};

NavMesh::NavMesh(const Tilemap &tm, const Tileset &ts) : _navmesh(tm.w * tm.h) {

  find_edges(tm, ts);
  // Populate the edges and weights in the _navmesh
  WeightMap weightmap = get(edge_weight, _navmesh);
  for (std::size_t j = 0; j < _edges.size(); ++j) {
    edge_descriptor e;
    bool inserted;
    boost::tie(e, inserted) =
        add_edge(_edges[j].first, _edges[j].second, _navmesh);
    weightmap[e] = _weights[j];
  }

  find_path(17, 225);
}

void NavMesh::find_edges(const Tilemap &tm, const Tileset &ts) {
  for (int xx = 0; xx < tm.w; ++xx) {
    for (int yy = 0; yy < tm.h; ++yy) {
      auto tile_index = tm.tile_index(xx, yy);
      const auto &tile = tm.get_tile(ts, tile_index);
      if (tile.collides) {
        continue;
      }

      // We'll see if we can move W, NW, N or NE. No need to check other
      // directions since our adjacency list has undirected edges

      // Check moving W
      if (xx > 0) {
        check_if_tiles_connected(tm, ts, tile_index, tm.tile_index(xx - 1, yy));
      }

      // Check moving NW
      //      if ((xx > 0) && (yy > 0)) {
      //        check_if_tiles_connected(tm, ts, tile_index,
      //                                 tm.tile_index(xx - 1, yy - 1));
      //      }

      // Check moving N
      if (yy > 0) {
        check_if_tiles_connected(tm, ts, tile_index, tm.tile_index(xx, yy - 1));
      }

      // Check moving NE
      //      if ((xx < (tm.w - 1)) && (yy > 0)) {
      //        check_if_tiles_connected(tm, ts, tile_index,
      //                                 tm.tile_index(xx + 1, yy - 1));
      //      }
    }
  }
}

void NavMesh::check_if_tiles_connected(const Tilemap &tm, const Tileset &ts,
                                       int from_tile_index, int to_tile_index) {
  const auto &to_tile = tm.get_tile(ts, to_tile_index);
  if (!to_tile.collides) {
    // We can move in between from_tile and _to_tile
    _edges.emplace_back(edge{from_tile_index, to_tile_index});
    // For now, all weights are the same. Maybe later add more/less favourable
    // tile types for walking on.
    _weights.emplace_back(1.0f);
  }
}

void NavMesh::find_path(vertex start, vertex goal) {
  vector<navmesh_t::vertex_descriptor> p(num_vertices(_navmesh));
  vector<cost> d(num_vertices(_navmesh));
  try {
    // call astar named parameter interface
    astar_search_tree(
        _navmesh, start, distance_heuristic<navmesh_t, cost>(goal),
        predecessor_map(
            make_iterator_property_map(p.begin(), get(vertex_index, _navmesh)))
            .distance_map(make_iterator_property_map(
                d.begin(), get(vertex_index, _navmesh)))
            .visitor(astar_goal_visitor<vertex>(goal)));
  } catch (found_goal fg) { // found a path to the goal
    list<vertex> shortest_path;
    for (vertex v = goal;; v = p[v]) {
      shortest_path.push_front(v);
      if (p[v] == v)
        break;
    }

    cout << "Shortest path from " << start << " to " << goal << ": ";
    list<vertex>::iterator spi = shortest_path.begin();
    cout << start;
    for (++spi; spi != shortest_path.end(); ++spi)
      cout << " -> " << *spi;
    cout << endl << "Total travel time: " << d[goal] << endl;
  }
}
