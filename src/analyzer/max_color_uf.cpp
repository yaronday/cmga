#include "max_color_uf.hpp"

UnionFindColorGrid::UnionFindColorGrid(MatFileHandler &handler) : ColorGrid(handler) {
  maxColorSet.clear();     // track max color codes
  colorRegionsMap.clear(); // track regions for each color
  visualizerEn = false;
  forceVis = FORCE_VIS;
  connColor = CONN_COLOR;
}

int UnionFindColorGrid::calcMaxConnectedColor() { return 0; }

int UnionFindColorGrid::calcMaxConnectedColor(Array2D<uint8_t> &mat, bool paint, bool colors,
                                              const std::string &filepath, bool crop) {

  init(mat);

  // Reset result state for every invocation.
  maxSize = 0;
  maxColorSet.clear();
  colorRegionsMap.clear();

  const int matSize = n * m;

  UnionFind uf(matSize);

  // Cache the contiguous matrix data pointer.
  const uint8_t *mat_data = mat.data();

  // 1: Build all connected components.
  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < m; ++col) {
      const int currCell = row * m + col;
      const uint8_t currValue = mat_data[currCell];

      updateValueStats(currValue);

      processAdjCells(row, col, mat, currCell, currValue, uf);
    }
  }

  // 2: Find all maximum-sized components using final roots.
  for (int i = 0; i < matSize; ++i) {
    if (uf.parent[i] != i)
      continue;

    const int componentSize = uf.size[i];

    if (componentSize > maxSize) {
      maxSize = componentSize;
      maxColorSet.clear();
      colorRegionsMap.clear();
    }

    if (componentSize == maxSize) {
      // Safe because unions are performed only between equal colors.
      const uint8_t componentColor = mat_data[i];

      maxColorSet.insert(componentColor);
      colorRegionsMap[componentColor].insert(i);
    }
  }

  // Select the smallest color among tied maximum colors.
  maxColor = *maxColorSet.begin();

  updateMfhFilename(filepath);

  if (visualizerEn) {
    if (forceVis || std::max(m, n) <= VIS_MAT_THR) {
      console::formatTxt("Visualizer activated!\n", LIGHT_MAGENTA);
      visualizeUF(uf, maxSize, filename, connColor);
    } else {
      console::formatTxt(err::getErrMsg(ErrCode::GRAPHVIZ_MAT_LIMIT_REACHED, err::ErrorContext()),
                         LIGHT_MAGENTA);
    }
  }

  bool extraInfoFile = false;

  auto cellCond = [&](int r, int c) -> int { return uf.getSize(r * m + c) == maxSize; };

  auto getCellValue = [&](int r, int c) -> char { return utils::itoc(mat_data[r * m + c]); };

  if (paint) {
    paintResultsArea(mat, colors, cellCond);
  } else {
    if (crop) {
      fWriteCropped(filename, uf, mat, n, m, cellCond, getCellValue);
    } else {
      const std::string fpath = updatePath(filename, maxSize, maxColor, algo);
      fWriteAll(fpath, n, m, cellCond, getCellValue);
    }

    extraInfoFile = true;
  }

  displayMaxSize(algo);

  if (maxColorSet.size() > 1) {
    notifyMaxColorRegions(filename, maxSize, maxColorSet, colorRegionsMap, extraInfoFile);
  }

  return maxSize;
}

void UnionFindColorGrid::visualizeUF(UnionFind &uf, int maxSize, const std::string &filename,
                                     const std::string &connColor) {
  // Requires: Graphviz.

  std::string filePathStr =
      std::format("{}{}{}{}{}", fRemoveExt(filename), OUT_MAX, maxSize, F_MAXCOLOR_PRFIX, maxColor);
  std::string dotFilePathStr = filePathStr + "_UF_tree." + DOT_;
  std::string outFilePathStr = filePathStr + "_UF_tree." + SVG_;

  std::ofstream dotFile(dotFilePathStr);
  if (!dotFile.is_open()) {
    return;
  }

  dotFile << "digraph UnionFindTree " << DOTFILEOPENER;
  dotFile << std::format("{}node [fontname=\"{}\"];\n", FOUR_SPACES, FONT_NAME);
  dotFile << std::format("{}{}\"{}\"];\n", FOUR_SPACES, GRAPHVIZ_CFG,
                         maxSize >= TREEPACK_THR ? "array_2" : "array");

  for (int i = 0; i < static_cast<int>(uf.parent.size()); ++i) {
    int root = uf.find(i);
    const char *region_color = getRegionColor(root, colorRegionsMap);

    if (root == i) {
      dotFile << std::format("{}{} [style=filled, fillcolor=\"{}\", "
                             "fontcolor=\"{}\", color=\"{}\", fontsize=\"{}\"];\n",
                             FOUR_SPACES, i, region_color, FONT_COLOR, R_STROKE_COLOR, R_FONT_SIZE);
    } else {
      dotFile << std::format("{}{} [style=filled, fillcolor=\"{}\", fontcolor=\"{}\"];\n",
                             FOUR_SPACES, i, region_color, FONT_COLOR);

      dotFile << std::format("{}{}{}{} [color={}];\n", FOUR_SPACES, i, SHORT_ARROW_, root,
                             connColor);
    }
  }

  dotFile << DOTFILECLOSER;
  dotFile.close();

  // Render an image from a dot file
  std::string renderCmd =
      std::format("{} -T{} \"{}\" -o \"{}\"", DOT_, SVG_, dotFilePathStr, outFilePathStr);

  try {
    if (system(renderCmd.c_str())) {
      throw std::runtime_error(RENDER_CMD_FAILED + renderCmd);
    }
  } catch (const std::exception &e) {
    err::handle(ErrCode::GENERIC_EXCEPTION, e);
    exit(1);
  }
}

// Notify extra max colors regions (unique method for UF algorithm)
// Useful for multiple equally-sized max regions with different color codes
void UnionFindColorGrid::notifyMaxColorRegions(
    const std::string &filename, int maxSize, const std::unordered_set<int> &maxColorSet,
    const std::unordered_map<int, std::unordered_set<int>> &colorRegionsMap, bool extraFile) {
  std::string content = std::format("{}{}:\n", CODES_W_MAX_SIZE, maxSize);
  // Iterate through all color codes in maxColorSet
  for (int colorCode : maxColorSet) {
    const auto &regions = colorRegionsMap.at(colorCode); // Retrieve regions for color code
    content += std::format("{}{}{}", COLOR_CODE_TXT, colorCode,
                           (regions.size() > 1 ? MULTIPLE_REGIONS : "\n"));
  }
  console::formatTxt(content, LIGHT_CYAN);

  if (extraFile)
    fWrite(content, updatePath(filename, maxSize, algo), "additional info file", true);
}

const char *UnionFindColorGrid::getRegionColor(
    int root, const std::unordered_map<int, std::unordered_set<int>> &colorRegionsMap) const {
  for (const auto &[colorCode, regions] : colorRegionsMap) {
    if (regions.find(root) != regions.end()) {
      return getGraphvizColorFromKey(colorCode);
    }
  }
  return "white";
}

void UnionFindColorGrid::processAdjCells(int row, int col, const Array2D<uint8_t> &mat,
                                         int currCell, uint8_t currValue, UnionFind &uf) {
  const uint8_t *data = mat.data();

  for (size_t i = 0; i < dr.size(); ++i) {
    const int newRow = row + dr[i];
    const int newCol = col + dc[i];

    // Retain upper-left, up, upper-right and left.
    if (newRow > row || (newRow == row && newCol >= col)) {
      continue;
    }

    if (!isValid(newRow, newCol, n, m)) {
      continue;
    }

    const int adjCell = newRow * m + newCol;

    if (data[adjCell] == currValue) {
      uf.unite(adjCell, currCell);
    }
  }
}

// For --crop option (only write each bounding box of equally-sized max region
// to separate file)
void UnionFindColorGrid::fWriteCropped(const std::string &filename, UnionFind &uf,
                                       Array2D<uint8_t> &mat, int n, int m,
                                       const std::function<bool(int, int)> &cellCondition,
                                       const std::function<char(int, int)> &getCellValue) {

  // Iterate through all color codes in maxColorSet
  for (int colorCode : maxColorSet) {
    // Iterate through all regions corresponding to this color code
    const auto &regions = colorRegionsMap.at(colorCode);
    for (int root : regions) {

      auto regionCond = [&](int r, int c) -> int {
        const int cell = r * m + c;
        return uf.find(cell) == root && cellCondition(r, c);
      };

      auto compositeCond = [&](int r, int c) -> bool {
        return regionCond(r, c) && (mat[r][c] == colorCode);
      };

      int minRow = n, maxRow = -1, minCol = m, maxCol = -1;
      calcBoundingBox(minRow, maxRow, minCol, maxCol, n, m, regionCond);

      // Construct a unique file name based on the bounding box coordinates
      const std::string filepath = updatePath(filename, maxSize, colorCode, minRow,
                                              alignCol(minCol), maxRow, alignCol(maxCol), algo);

      fWriteResults(filepath, n, m, compositeCond, getCellValue, true, minRow, maxRow, minCol,
                    maxCol);
    }
  }
}
