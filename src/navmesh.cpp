#include "navmesh.hpp"
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
  distance_heuristic(Vertex goal, int w)
      : _goal_x(goal % w), _goal_y(goal / w), _width(w) {}
  CostType operator()(Vertex u) {

    int u_x = u % _width;
    int u_y = u / _width;

    CostType dx = _goal_x - u_x;
    CostType dy = _goal_y - u_y;
    return ::sqrt(dx * dx + dy * dy);
  }

private:
  int _width;
  int _goal_x;
  int _goal_y;
};

struct found_goal {}; // exception for path finding termination

NavMesh::astar_goal_visitor::astar_goal_visitor(vertex goal) : m_goal(goal) {}
void NavMesh::astar_goal_visitor::examine_vertex(vertex u, const navmesh_t &g) {
  if (u == m_goal)
    throw found_goal();
}

NavMesh::NavMesh(const Tilemap &tm, const Tileset &ts) : _navmesh(tm.w * tm.h) {

  find_edges(tm, ts);

  find_path(17, 225, tm.w);
}

void NavMesh::find_edges(const Tilemap &tm, const Tileset &ts) {
  WeightMap weightmap = get(edge_weight, _navmesh);
  float sqrt_two = ::sqrtf(2.0f);
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
        check_if_tiles_connected(tm, ts, tile_index, tm.tile_index(xx - 1, yy),
                                 weightmap, 1.0f);
      }

      // Check moving NW
      if ((xx > 0) && (yy > 0)) {
        check_if_tiles_connected(tm, ts, tile_index,
                                 tm.tile_index(xx - 1, yy - 1), weightmap,
                                 sqrt_two);
      }

      // Check moving N
      if (yy > 0) {
        check_if_tiles_connected(tm, ts, tile_index, tm.tile_index(xx, yy - 1),
                                 weightmap, 1.0f);
      }

      // Check moving NE
      if ((xx < (tm.w - 1)) && (yy > 0)) {
        check_if_tiles_connected(tm, ts, tile_index,
                                 tm.tile_index(xx + 1, yy - 1), weightmap,
                                 sqrt_two);
      }
    }
  }
}

void NavMesh::check_if_tiles_connected(const Tilemap &tm, const Tileset &ts,
                                       int from_tile_index, int to_tile_index,
                                       WeightMap &weightmap, cost weight) {
  const auto &to_tile = tm.get_tile(ts, to_tile_index);
  if (!to_tile.collides) {
    edge_descriptor e;
    bool inserted;
    boost::tie(e, inserted) =
        add_edge(from_tile_index, to_tile_index, _navmesh);
    weightmap[e] = weight;
  }
}

void NavMesh::find_path(vertex start, vertex goal, int width) {
  vector<navmesh_t::vertex_descriptor> p(num_vertices(_navmesh));
  vector<cost> d(num_vertices(_navmesh));
  try {
    // call astar named parameter interface
    astar_search_tree(
        _navmesh, start, distance_heuristic<navmesh_t, cost>(goal, width),
        predecessor_map(
            make_iterator_property_map(p.begin(), get(vertex_index, _navmesh)))
            .distance_map(make_iterator_property_map(
                d.begin(), get(vertex_index, _navmesh)))
            .visitor(astar_goal_visitor(goal)));
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
