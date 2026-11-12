#pragma once

#include "color_grid.hpp"
#include "common/array2d.hpp"
#include "io/matfile_handler.hpp"

class DfsColorGrid : public ColorGrid {
private:
  const std::string algo = DFS_;

public:
  DfsColorGrid() = delete;

  explicit DfsColorGrid(MatFileHandler &handler);

  virtual int calcMaxConnectedColor() override;

  int calcMaxConnectedColor(Array2D<uint8_t> &mat, bool paint = true, bool colors = true,
                            const std::string &filepath = "", bool crop = false);

protected:
  int dfs(const Array2D<uint8_t> &mat, uint8_t curr_value, int row, int col,
          std::vector<uint8_t> &checked, std::vector<std::pair<int, int>> &coord_list);
};