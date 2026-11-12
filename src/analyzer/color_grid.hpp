#pragma once

#include <functional>
#include <set> // IWYU pragma: keep
#include <vector>
#include <unordered_set> // IWYU pragma: keep

#include "common/array2d.hpp"
#include "io/matfile_handler.hpp"
#include "support/error_handler.hpp" // IWYU pragma: keep
#include "support/utils.hpp"         // IWYU pragma: keep

class ColorGrid {
protected:
  MatFileHandler &mfh;

  bool isValid(int row, int col, int n, int m);

  std::string updatePath(const std::string &filename, int maxSize, int maxColor,
                         const std::string &algo, bool txt = EN_TXT);

  std::string updatePath(const std::string &filename, int maxSize, const std::string &algo,
                         bool txt = EN_TXT);

  std::string updatePath(const std::string &filename, int maxSize, int colorCode, int minRow,
                         int minCol, int maxRow, int maxCol, const std::string &algo,
                         bool txt = EN_TXT);

  // calc bounding box or minimum bounding rectangle (MBR)
  void calcBoundingBox(int &minRow, int &maxRow, int &minCol, int &maxCol, int n, int m,
                       const std::function<bool(int, int)> &cellCondition);

  int alignCol(int &col);

public:
  int n, m, maxSize, maxColor, minVal, maxVal;
  std::string filename;

  // 8-neighbour directions for DFS / UF traversal
  std::vector<int> dr = {1, -1, 0, 0, 1, -1, -1, 1};
  std::vector<int> dc = {0, 0, 1, -1, -1, -1, 1, 1};

  virtual int calcMaxConnectedColor() = 0;

  // Default constructor deleted to enforce passing MatFileHandler
  ColorGrid() = delete;

  // Constructor with MatFileHandler initialization
  explicit ColorGrid(MatFileHandler &handler);

  void init(Array2D<uint8_t> &mat);
  void setFileName(const std::string &path, std::vector<int> &matrixParams);

  void paintResultsArea(Array2D<uint8_t> &matrix, bool colors,
                        const std::function<bool(int, int)> cellCondition) const;

  void updateValueStats(int value);

  void displayMaxSize(const std::string &) const;

  template <typename ConditionFunc, typename ValueFunc>
  void fWriteAll(const std::string &filepath, int n, int m, ConditionFunc cellCondition,
                 ValueFunc getCellValue) {
    fWriteResults(filepath, n, m, cellCondition, getCellValue, false);
  }

  void fWriteCropped(const std::string &filename, int n, int m,
                     const std::function<bool(int, int)> &cellCondition,
                     const std::function<char(int, int)> &getCellValue, const std::string &algo);

  template <typename ConditionFunc, typename ValueFunc>
  void fWriteResults(const std::string &filepath, int n, int m, ConditionFunc cellCondition,
                     ValueFunc getValue, bool cropped, int minRow = 0, int maxRow = -1,
                     int minCol = 0, int maxCol = -1) {

    if (cropped) {
      maxRow = (maxRow == -1) ? n - 1 : maxRow;
      maxCol = (maxCol == -1) ? m - 1 : maxCol;
    } else {
      minRow = 0;
      maxRow = n - 1;
      minCol = 0;
      maxCol = m - 1;
    }

    auto buffer = mfh.fillBuffer(cellCondition, getValue, minRow, maxRow, minCol, maxCol);
    fWrite(buffer, filepath, DEFAULT_INFO, true);
  }

  // update MatFileHandler filename member
  void updateMfhFilename(const std::string &filepath);
};
