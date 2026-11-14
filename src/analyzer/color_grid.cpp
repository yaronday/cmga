#include "color_grid.hpp"

ColorGrid::ColorGrid(MatFileHandler &handler)
    : mfh(handler), n(1), m(1), maxSize(1), maxColor(1), minVal(INT_MAX), maxVal(INT_MIN),
      filename("") {}

void ColorGrid::init(Array2D<uint8_t> &mat) {
  n = static_cast<int>(mat.rows());
  m = static_cast<int>(mat.cols());
  maxColor = 1;
  maxSize = 1;
}

bool ColorGrid::isValid(int row, int col, int n, int m) {
  return (row >= 0 && row < n && col >= 0 && col < m);
}

void ColorGrid::setFileName(const std::string &path, std::vector<int> &matParams) {
  filename =
      path.empty() ? std::format("{}/{}", mfh.destpath, mfh.updateStrByIntVect(matParams)) : path;
}

std::string ColorGrid::updatePath(const std::string &filename, int maxSize, int maxColor,
                                  const std::string &algo, bool txt) {
  return fGenPath(filename,
                  std::format("{}{}{}{}_{}", OUT_MAX, maxSize, F_MAXCOLOR_PRFIX, maxColor, algo),
                  txt);
}

std::string ColorGrid::updatePath(const std::string &filename, int maxSize, const std::string &algo,
                                  bool txt) {
  return fGenPath(filename, std::format("{}{}_{}{}", OUT_MAX, maxSize, algo, EXTRA_INFO), txt);
}

std::string ColorGrid::updatePath(const std::string &filename, int maxSize, int colorCode,
                                  int minRow, int minCol, int maxRow, int maxCol,
                                  const std::string &algo, bool txt) {
  std::string suffix = format("{}{}{}{}{}{}{}({},{}){}({},{})", OUT_MAX, maxSize, CROPPED_, algo,
                              COLOR_, colorCode, TL_PREF, minRow, minCol, BR_PREF, maxRow, maxCol);

  std::string notification =
      format("{}{}{}{}({}, {}), {}({}, {}).\n", RECT_BOUNDARIES, algo, CROP_COORDINATES, TOPLEFT_,
             minRow, minCol, BOTTOMRIGHT_, maxRow, maxCol);

  console::formatTxt(notification, LIGHT_MAGENTA);
  return fGenPath(filename, suffix, txt);
}

int ColorGrid::alignCol(int &col) { return (col % 2) ? 2 * col + 1 : 2 * col; }

void ColorGrid::paintResultsArea(Array2D<uint8_t> &mat, bool colors,
                                 std::function<bool(int, int)> cellCondition) const {
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < m; c++) {
      uint8_t currValue = mat[r][c];
      std::string currValueStr = std::to_string(static_cast<int>(currValue));

      if (cellCondition(r, c)) {
        if (colors)
          console::formatTxt(currValueStr, getColorFromKey(currValue), " ");
        else
          console::formatTxt(std::string(1, getSymbolFromKey(currValue)), DEFAULT_COLOR, " ");
      } else {
        console::formatTxt(currValueStr, DEFAULT_COLOR, " ");
      }
    }
    std::cout << std::endl;
  }
}

void ColorGrid::calcBoundingBox(int &minRow, int &maxRow, int &minCol, int &maxCol, int n, int m,
                                const std::function<bool(int, int)> &cellCondition) {
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < m; c++) {
      if (cellCondition(r, c)) {
        minRow = std::min(minRow, r);
        maxRow = std::max(maxRow, r);
        minCol = std::min(minCol, c);
        maxCol = std::max(maxCol, c);
      }
    }
  }
}

void ColorGrid::updateValueStats(int value) {
  minVal = std::min(value, minVal);
  maxVal = std::max(value, maxVal);
}

void ColorGrid::displayMaxSize(const std::string &algo) const {
  console::formatTxt(format("{}({}): {}\n", MAX_CON_COLORS, algo, maxSize), LIGHT_CYAN);
}

void ColorGrid::fWriteCropped(const std::string &filename, int n, int m,
                              const std::function<bool(int, int)> &cellCondition,
                              const std::function<char(int, int)> &getCellValue,
                              const std::string &algo) {

  int minRow = n, maxRow = -1, minCol = m, maxCol = -1;
  calcBoundingBox(minRow, maxRow, minCol, maxCol, n, m, cellCondition);

  // Construct a unique file name based on the bounding box coordinates
  std::string filepath = updatePath(filename, maxSize, maxColor, minRow, alignCol(minCol), maxRow,
                                    alignCol(maxCol), algo);
  fWriteResults(filepath, n, m, cellCondition, getCellValue, true, minRow, maxRow, minCol, maxCol);
}

void ColorGrid::updateMfhFilename(const std::string &filepath) {
  mfh.matInfo = {n, m, minVal, maxVal};
  setFileName(filepath, mfh.matInfo);
  mfh.filename = this->filename;
}
