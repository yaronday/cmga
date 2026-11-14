#include "max_color_dfs.hpp"

DfsColorGrid::DfsColorGrid(MatFileHandler &handler) : ColorGrid(handler) {}

int DfsColorGrid::calcMaxConnectedColor() { return 0; }

int DfsColorGrid::calcMaxConnectedColor(Array2D<uint8_t> &mat, bool paint, bool colors,
                                        const std::string &filepath, bool crop) {
  init(mat);

  int curr_max = 1;
  std::vector<std::pair<int, int>> xy_coord_list;
  const size_t total_cells = static_cast<size_t>(n) * m;
  std::vector<uint8_t> checked(total_cells, 0);

  std::vector<std::pair<int, int>> curr_xy_coord_list;

  const uint8_t *mat_data = mat.data();

  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < m; ++col) {
      const size_t flat_idx = static_cast<size_t>(row) * m + col;

      const uint8_t currValue = mat_data[flat_idx];

      updateValueStats(currValue);

      if (checked[flat_idx] == 0) {

        curr_xy_coord_list.clear();

        curr_max = dfs(mat, currValue, row, col, checked, curr_xy_coord_list);

        if (maxSize < curr_max) {
          maxSize = curr_max;
          maxColor = currValue;

          xy_coord_list.swap(curr_xy_coord_list);
        }
      }
    }
  }

  std::vector<uint8_t> best_component_mask(total_cells, 0);

  for (const auto &[r, c] : xy_coord_list) {
    const size_t idx = static_cast<size_t>(r) * m + c;
    best_component_mask[idx] = 1;
  }

  updateMfhFilename(filepath);

  const size_t cols = static_cast<size_t>(m);

  auto cellCond = [&best_component_mask, cols](int r, int c) -> bool {
    return best_component_mask[static_cast<size_t>(r) * cols + c] != 0;
  };

  auto getCellValue = [&](int r, int c) -> char { return utils::itoc(maxColor); };

  if (paint) {
    paintResultsArea(mat, colors, cellCond);
  } else {
    if (crop) {
      fWriteCropped(filename, n, m, cellCond, getCellValue, algo);
    } else {
      const std::string out_filepath = updatePath(filename, maxSize, maxColor, algo);

      fWriteAll(out_filepath, n, m, cellCond, getCellValue);
    }
  }

  displayMaxSize(algo);

  return maxSize;
}

int DfsColorGrid::dfs(const Array2D<uint8_t> &mat, uint8_t curr_value, int row, int col,
                      std::vector<uint8_t> &checked, std::vector<std::pair<int, int>> &coord_list) {
  if (!isValid(row, col, n, m)) {
    return 0;
  }

  const size_t flat_idx = static_cast<size_t>(row) * m + col;

  if (checked[flat_idx] != 0 || mat.data()[flat_idx] != curr_value) {
    return 0;
  }

  checked[flat_idx] = 1;
  coord_list.emplace_back(row, col);

  int res = 1;

  for (size_t i = 0; i < dr.size(); ++i) {
    res += dfs(mat, curr_value, row + dr[i], col + dc[i], checked, coord_list);
  }

  return res;
}