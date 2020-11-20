#include "navmesh.hpp"
#include <iostream>

using boost::astar_heuristic;
using boost::edge_weight;
using boost::graph_traits;
using boost::vertex_index;
using std::cout;
using std::endl;

// euclidean distance heuristic
template <class Graph, class CostType>
class distance_heuristic : public astar_heuristic<Graph, CostType> {
public:
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  distance_heuristic(Vertex goal, const Tilemap &tm)
      : _tm(tm), _goal_coords(tm.tile_coords(goal)) {}
  CostType operator()(Vertex u) {
    Tilecoords u_coords = _tm.tile_coords(u);

    CostType dx = _goal_coords.tile_x - u_coords.tile_x;
    CostType dy = _goal_coords.tile_y - u_coords.tile_y;
    return ::sqrt(dx * dx + dy * dy);
  }

private:
  const Tilemap &_tm;
  Tilecoords _goal_coords;
};

struct found_goal {}; // exception for path finding termination

NavMesh::astar_goal_visitor::astar_goal_visitor(vertex goal) : m_goal(goal) {}
void NavMesh::astar_goal_visitor::examine_vertex(vertex u, const navmesh_t &g) {
  if (u == m_goal)
    throw found_goal();
}

NavMesh::NavMesh(const Tilemap &tm, const Tileset &ts)
    : _navmesh(tm.w * tm.h), tm(tm), ts(ts) {

  find_edges();

  auto shortestPath = find_shortest_path(17, 225);
  cout << "Start";
  for (auto tile_index : shortestPath) {
    cout << " -> " << tile_index;
  }
  cout << endl;
}

list<int> NavMesh::find_shortest_path(vertex start, vertex goal) const {
  list<int> shortest_path;
  vector<navmesh_t::vertex_descriptor> p(num_vertices(_navmesh));
  vector<cost> d(num_vertices(_navmesh));
  try {
    // call astar named parameter interface
    astar_search_tree(
        _navmesh, start, distance_heuristic<navmesh_t, cost>(goal, tm),
        predecessor_map(
            make_iterator_property_map(p.begin(), get(vertex_index, _navmesh)))
            .distance_map(make_iterator_property_map(
                d.begin(), get(vertex_index, _navmesh)))
            .visitor(astar_goal_visitor(goal)));
  } catch (found_goal fg) { // found a path to the goal
    for (vertex v = goal;; v = p[v]) {
      if (p[v] == v)
        break;
      shortest_path.push_front(v);
    }
  }
  return shortest_path;
}

list<int> NavMesh::find_shortest_path(Tilecoords start, Tilecoords goal) const {
  return find_shortest_path(tm.tile_index(start), tm.tile_index(goal));
}

void NavMesh::find_edges() {
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
        check_if_tiles_connected(tile_index, tm.tile_index(xx - 1, yy),
                                 vector<int>(), weightmap, 1.0f);
      }

      // Check moving NW. We also need to make sure the tiles on either side
      // of the diagonal movement do not collide
      if ((xx > 0) && (yy > 0)) {
        check_if_tiles_connected(
            tile_index, tm.tile_index(xx - 1, yy - 1),
            vector<int>{tm.tile_index(xx - 1, yy), tm.tile_index(xx, yy - 1)},
            weightmap, sqrt_two);
      }

      // Check moving N
      if (yy > 0) {
        check_if_tiles_connected(tile_index, tm.tile_index(xx, yy - 1),
                                 vector<int>(), weightmap, 1.0f);
      }

      // Check moving NE. We also need to make sure the tiles on either side
      // of the diagonal movement do not collide
      if ((xx < (tm.w - 1)) && (yy > 0)) {
        check_if_tiles_connected(
            tile_index, tm.tile_index(xx + 1, yy - 1),
            vector<int>{tm.tile_index(xx + 1, yy), tm.tile_index(xx, yy - 1)},
            weightmap, sqrt_two);
      }
    }
  }
}

void NavMesh::check_if_tiles_connected(
    int from_tile_index, int to_tile_index,
    vector<int> other_tile_indexes_which_must_not_collide, WeightMap &weightmap,
    cost weight) {

  bool all_tiles_do_not_collide = true;

  if (tm.get_tile(ts, to_tile_index).collides) {
    all_tiles_do_not_collide = false;
  }

  if (all_tiles_do_not_collide) {
    for (auto ti : other_tile_indexes_which_must_not_collide) {
      const auto &tile = tm.get_tile(ts, ti);
      if (tile.collides) {
        all_tiles_do_not_collide = false;
      }
    }
  }

  if (all_tiles_do_not_collide) {
    edge_descriptor e;
    bool inserted;
    boost::tie(e, inserted) =
        add_edge(from_tile_index, to_tile_index, _navmesh);
    weightmap[e] = weight;
  }
}
