#pragma once

#include "analyzer/max_color_dfs.hpp"
#include "analyzer/max_color_uf.hpp"
#include "cli/cli_utils.hpp" // IWYU pragma: keep
#include "common/timer.hpp"  // IWYU pragma: keep

void solveMccg(DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG, Array2D<uint8_t> &mat,
               const std::string &algo, bool &paint, bool &colors, bool crop,
               const std::string &filepath = "");

void processData(DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG, MatFileHandler &mfh,
                 const std::string &algo, bool &paint, bool &colors, bool crop);