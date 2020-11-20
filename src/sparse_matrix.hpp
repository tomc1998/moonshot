#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template <typename T> struct SparseMatrixCell {
  T val;
  int x;
  int y;
};

/** Sparse matrix of T
 * TODO optimise - compression?*/
template <typename T> struct SparseMatrix {
  std::vector<SparseMatrixCell<T>> values;

  /** Iterator over SparseMatrixCell<T> */
  inline auto begin() { return values.begin(); }

  /** Iterator over SparseMatrixCell<T> */
  inline auto end() { return values.end(); }

  /** nullptr if not found */
  inline T *get(int x, int y) {
    const auto res = std::find_if(begin(), end(), [x, y](const auto &cell) {
      return cell.x == x and cell.y == y;
    });
    return (res == end()) ? nullptr : &res->val;
  }

  inline void remove(int x, int y) {
    auto to_remove =
        std::remove_if(values.begin(), values.end(), [x, y](auto &cell) {
          return cell.x == x && cell.y == y;
        });
    values.erase(to_remove, values.end());
  }

  inline void set(int x, int y, T &&val) {
    for (auto &v : values) {
      if (v.x == x and v.y == y) {
        v.val = val;
        return;
      }
    }
    values.push_back({val, x, y});
  }
};

