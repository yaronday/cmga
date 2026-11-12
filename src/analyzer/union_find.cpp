#include "union_find.hpp"

// using path halving
[[nodiscard]] int UnionFind::find(int x) {
  while (parent[x] != x) {
    parent[x] = parent[parent[x]];
    x = parent[x];
  }
  return x;
}

UnionFind::UnionFind(int n) {
  parent.resize(n);
  size.resize(n);
  for (int i = 0; i < n; ++i) {
    parent[i] = i;
    size[i] = 1;
  }
}

void UnionFind::unite(int x, int y) {
  int rootX = find(x);
  int rootY = find(y);
  if (rootX != rootY) {
    if (size[rootX] < size[rootY]) {
      std::swap(rootX, rootY);
    }
    parent[rootY] = rootX;
    size[rootX] += size[rootY];
  }
}

[[nodiscard]] int UnionFind::getSize(int x) { return size[find(x)]; }
