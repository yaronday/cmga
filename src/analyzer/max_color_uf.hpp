#pragma once

#include "color_grid.hpp"
#include "union_find.hpp"

#include <fstream> // IWYU pragma: keep
#include <string>

class UFColorGrid : public ColorGrid {
private:
  const std::string algo = UF_;

public:
  bool visualizerEn;     // specific to UF
  bool forceVis;         // Force visualizer even if matrix exceeds threshold
  std::string connColor; // node connector color (x11 color str or hex code)

  UFColorGrid() = delete;

  explicit UFColorGrid(MatFileHandler &handler);

  virtual int calcMaxConnectedColor() override;

  int calcMaxConnectedColor(Array2D<uint8_t> &mat, bool paint = true, bool colors = true,
                            const std::string &filepath = "", bool crop = false);

protected:
  std::unordered_set<int> maxColorSet;

  std::unordered_map<int, std::unordered_set<int>> colorRegionsMap;

  void visualizeUF(UnionFind &uf, int maxSize, const std::string &filename,
                   const std::string &connColor = CONN_COLOR);

  const char *
  getRegionColor(int root,
                 const std::unordered_map<int, std::unordered_set<int>> &colorRegionsMap) const;

  void
  notifyMaxColorRegions(const std::string &filename, int maxSize,
                        const std::unordered_set<int> &maxColorSet,
                        const std::unordered_map<int, std::unordered_set<int>> &colorRegionsMap,
                        bool extraFile = false);

  void processAdjCells(int row, int col, const Array2D<uint8_t> &mat, int currCell,
                       uint8_t currValue, UnionFind &uf);

  void fWriteCropped(const std::string &filename, UnionFind &uf, Array2D<uint8_t> &mat, int n,
                     int m, const std::function<bool(int, int)> &cellCondition,
                     const std::function<char(int, int)> &getCellValue);
};
