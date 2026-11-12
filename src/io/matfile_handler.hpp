#pragma once

#include "common/array2d.hpp"
#include "file_utils.hpp"    // IWYU pragma: keep
#include "support/utils.hpp" // IWYU pragma: keep

class MatFileHandler {

public:
  int min_rows;
  int max_rows;
  int row_inc;
  int min_cols;
  int max_cols;
  int col_inc;
  int min_v;
  int max_v;
  bool sqmat;

  std::string destpath;
  bool confirm;
  bool txt;
  bool overwrite;

  Array2D<uint8_t> currMat;

  int parsedMatIdx; // for matString

  std::vector<int> matInfo;

  std::string filename;

  MatFileHandler();

  void updateMatParams(int min_rows = MIN_ROW_V, int max_rows = MAX_ROW_V, int row_inc = INC_V,
                       int min_cols = MIN_ROW_V, int max_cols = MAX_ROW_V, int col_inc = INC_V,
                       int min_v = MIN_V, int max_v = MAX_V);

  const std::string updatePath(int rows, int cols, int min_value, int max_value) const;

  const std::string updatePath(const std::string &filename) const;

  std::string updateStrByIntVect(std::vector<int>) const;

  void displayMat(const Array2D<uint8_t> &mat);

  void displayMatrices(const std::vector<Array2D<uint8_t>> &mat_vector);

  bool isValidMatSpec(int rows, int cols) const;

  Array2D<uint8_t> genRndMat(int rows, int cols, std::mt19937 gen) const;

  Array2D<uint8_t> genRndMatWrapper(int rows, int cols,
                                    std::optional<unsigned int> seed = std::nullopt) const;

  Array2D<uint8_t> parseMat(const std::string &matrixStr);

  std::vector<Array2D<uint8_t>> parseMatrices(const std::string &matricesStr);

  void calcSizeSqMatFiles() const;

  void calcSizeMatFiles() const;

  bool matLoader(Array2D<uint8_t> &matrix, const std::string &filepath);

  std::vector<std::pair<std::string, Array2D<uint8_t>>> fLoadMatrices();

  void fWriteRndMat(int rows, int cols);

  // write matrix -> buffer -> file
  void fWriteMat(const std::string &filename = "mat_check", bool skipUpdate = false);

  std::vector<char> fillBuffMat();

  std::vector<char> fillBuffRndMat(int rows, int cols, std::mt19937 gen);

  std::vector<char> fillBuffRndMat(int rows, int cols);

  // Helper function to initialize a buffer
  std::vector<char> bufferInit(int min_r, int max_r, int min_c, int max_c);

  template <typename ConditionFunc, typename ValueFunc>
  std::vector<char> fillBuffer(ConditionFunc cellCondition, ValueFunc getValue, int minRow,
                               int maxRow, int minCol, int maxCol) {

    auto buffer = bufferInit(minRow, maxRow, minCol, maxCol);

    const char delimiter = txt ? ' ' : ',';

    auto condValue = [&](int r, int c) { return cellCondition(r, c) ? getValue(r, c) : delimiter; };

    populateBuffer(buffer, minRow, maxRow, minCol, maxCol, delimiter, condValue);

    return buffer;
  }

  // General-purpose buffer population function
  template <typename ValueFunc>
  void populateBuffer(std::vector<char> &buffer, int minRow, int maxRow, int minCol, int maxCol,
                      char delimiter, ValueFunc getValue) {

    char *wPtr = buffer.data();

    for (int r = minRow; r <= maxRow; ++r) {
      if (minCol <= maxCol) {
        *wPtr++ = getValue(r, minCol);
        for (int c = minCol + 1; c <= maxCol; ++c) {
          *wPtr++ = delimiter;
          *wPtr++ = getValue(r, c);
        }
      }
      *wPtr++ = '\r';
      *wPtr++ = '\n';
    }
  }

  // Generate N x M matrix files
  void genMatFiles();

  // Generate N x N square matrix files
  void genSqMatFiles();

  void invokeGenMatFiles();
};