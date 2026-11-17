#include "main.hpp"

int main(int argc, char *argv[]) {
  bool paint = false;
  bool colors = true;
  bool crop = false;

  std::string algoChoice;

  bool visEn = false;
  bool forceVis = false;
  std::string connColor;

  std::signal(SIGINT, console::signalHandler);

  MatFileHandler mfh;
  std::string matStr;

  parseCLIargs(argc, argv, matStr, algoChoice, mfh, paint, colors, crop, visEn, forceVis,
               connColor);

  DfsColorGrid dfsCG(mfh);
  UFColorGrid ufCG(mfh);
  ufCG.visualizerEn = visEn;
  ufCG.forceVis = forceVis;
  ufCG.connColor = connColor;

  algoNotifier(algoChoice);

  visNotifier(algoChoice, visEn);

  if (matStr.empty()) {
    processData(dfsCG, ufCG, mfh, algoChoice, paint, colors, crop);
  } else {
    auto matList = mfh.parseMatrices(matStr);
    for (auto &mat : matList) {
      mfh.currMat = std::move(mat);
      mfh.parsedMatIdx++;
      timedSolver(dfsCG, ufCG, mfh.currMat, algoChoice, paint, colors, crop, {});
      mfh.fWriteMat(mfh.filename, true);
    }
  }
  return 0;
}

void solver(DfsColorGrid &dfsCG, Array2D<uint8_t> &mat, bool paint, bool colors, bool crop,
            const std::string &filepath) {
  dfsCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
}

void solver(UFColorGrid &ufCG, Array2D<uint8_t> &mat, bool paint, bool colors, bool crop,
            const std::string &filepath) {
  ufCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
}

void processData(DfsColorGrid &dfsCG, UFColorGrid &ufCG, MatFileHandler &mfh,
                 const std::string &algo, bool paint, bool colors, bool crop) {
  auto data = mfh.fLoadMatrices();

  if (data.empty())
    return;

  console::formatTxt(DATAPROC, LIGHT_CYAN);
  for (size_t k = 0; k < data.size(); ++k) {
    auto &[filepath, matrix] = data[k];
    mfh.currMat = std::move(matrix);
    console::formatTxt(format("\n{}){}\n", k + 1, filepath), LIGHT_CYAN);
    timedSolver(dfsCG, ufCG, mfh.currMat, algo, paint, colors, crop, filepath);
  }
}

void runSolver(DfsColorGrid &dfsCG, UFColorGrid &ufCG, Array2D<uint8_t> &mat,
               const std::string &algo, bool paint, bool colors, bool crop,
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

void timedSolver(DfsColorGrid &dfsCG, UFColorGrid &ufCG, Array2D<uint8_t> &mat,
                 const std::string &algo, bool paint, bool colors, bool crop,
                 const std::string &filepath) {

  if (algo == BOTH_) {
    timedSolver(DFS_, filepath, [&] { solver(dfsCG, mat, paint, colors, crop, filepath); });
    timedSolver(UF_, filepath, [&] { solver(ufCG, mat, paint, colors, crop, filepath); });
  } else {
    timedSolver(algo, filepath,
                [&] { runSolver(dfsCG, ufCG, mat, algo, paint, colors, crop, filepath); });
  }
}