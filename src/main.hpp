#pragma once

#include "analyzer/max_color_dfs.hpp"
#include "analyzer/max_color_uf.hpp"
#include "cli/cli_utils.hpp" // IWYU pragma: keep
#include "common/timer.hpp"  // IWYU pragma: keep

void solver(DfsColorGrid &dfsCG, Array2D<uint8_t> &mat, bool &paint, bool &colors, bool crop,
            const std::string &filepath);

void solver(UFColorGrid &ufCG, Array2D<uint8_t> &mat, bool &paint, bool &colors, bool crop,
            const std::string &filepath);

void runSolver(DfsColorGrid &dfsCG, UFColorGrid &ufCG, Array2D<uint8_t> &mat,
               const std::string &algo, bool &paint, bool &colors, bool crop,
               const std::string &filepath) {

  if (algo == DFS_) {
    solver(dfsCG, mat, paint, colors, crop, filepath);
  } else if (algo == UF_) {
    solver(ufCG, mat, paint, colors, crop, filepath);
  } else if (algo == BOTH_) {
    solver(dfsCG, mat, paint, colors, crop, filepath);
    solver(ufCG, mat, paint, colors, crop, filepath);
  }
}

void processData(DfsColorGrid &dfsCG, UFColorGrid &ufCG, MatFileHandler &mfh,
                 const std::string &algo, bool &paint, bool &colors, bool crop);

template <typename Solver>
void timedSolver(std::string_view algo, std::string_view filepath, Solver &&solverFn) {
  picotimer::timeit(format("Solver using {} for {}", algo, filepath),
                    std::forward<Solver>(solverFn), 1);
}

void timedSolver(DfsColorGrid &dfsCG, UFColorGrid &ufCG, Array2D<uint8_t> &mat,
                 const std::string &algo, bool &paint, bool &colors, bool crop,
                 const std::string &filepath) {

  if (algo == BOTH_) {
    timedSolver(DFS_, filepath, [&] { solver(dfsCG, mat, paint, colors, crop, filepath); });
    timedSolver(UF_, filepath, [&] { solver(ufCG, mat, paint, colors, crop, filepath); });
  } else {
    timedSolver(algo, filepath,
                [&] { runSolver(dfsCG, ufCG, mat, algo, paint, colors, crop, filepath); });
  }
}