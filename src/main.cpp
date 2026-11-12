#include "main.hpp"
#include "common/timer.hpp"

int main(int argc, char *argv[]) {
  bool paint = false;
  bool colors = true;
  bool crop = false;

  std::string algoChoice;

  bool visEn;
  bool forceVis;
  std::string connColor;

  signal(SIGINT, console::handleSignal);

  MatFileHandler mfh;

  std::string matStr;

  DfsColorGrid dfsCG(mfh);
  UnionFindColorGrid ufCG(mfh);

  // Parse CLI arguments
  handleArgs(argc, argv, matStr, algoChoice, mfh, paint, colors, crop, visEn, forceVis, connColor);

  ufCG.visualizerEn = visEn;
  ufCG.forceVis = forceVis;
  ufCG.connColor = connColor;

  algoNotifier(algoChoice);

  visualizerNotifier(algoChoice, visEn);

  if (matStr.empty())
    processData(dfsCG, ufCG, mfh, algoChoice, paint, colors, crop);
  else {
    std::vector<Array2D<uint8_t>> matList;
    mfh.overwrite = true;
    matList = mfh.parseMatrices(matStr);
    for (Array2D<uint8_t> &mat : matList) {
      mfh.currMat = mat;
      mfh.parsedMatIdx++;
      solveMccg(dfsCG, ufCG, mfh.currMat, algoChoice, paint, colors, crop);
      mfh.fWriteMat(mfh.filename, true);
    }
  }
  return 0;
}

void solveMccg(DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG, Array2D<uint8_t> &mat,
               const std::string &algo, bool &paint, bool &colors, bool crop,
               const std::string &filepath) {

  if (algo == DFS_ || algo == BOTH_) {
    dfsCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
  }
  if (algo == UF_ || algo == BOTH_) {
    ufCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
  }
}

void processData(DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG, MatFileHandler &mfh,
                 const std::string &algo, bool &paint, bool &colors, bool crop) {
  std::vector<std::pair<std::string, Array2D<uint8_t>>> data = mfh.fLoadMatrices();
  const int num_of_matrices = static_cast<int>(data.size());
  if (num_of_matrices > 0) {
    console::formatTxt(PROCESSING_DATA, LIGHT_CYAN);
    for (int k = 0; k < num_of_matrices; k++) {
      std::string filepath = data[k].first;
      mfh.currMat = std::move(data[k].second);
      console::formatTxt(format("\n{}){}\n", k + 1, filepath), LIGHT_CYAN);

      auto timed_solver = [&]() {
        solveMccg(dfsCG, ufCG, mfh.currMat, algo, paint, colors, crop, filepath);
      };
      picotimer::time_it(format("solveMccg using {} for {}", algo, filepath), timed_solver, 1);
    }
  }
}