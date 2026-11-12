#pragma once

#include <vector>

class UnionFind {
public:
  std::vector<int> parent;
  std::vector<int> size;

  UnionFind(int n);
  int find(int x);
  void unite(int x, int y);
  int getSize(int x);
};
