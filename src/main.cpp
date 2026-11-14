#include "main.hpp"

int main(int argc, char *argv[]) {
  bool paint = false;
  bool colors = true;
  bool crop = false;

  std::string algoChoice;

  bool visEn;
  bool forceVis;
  std::string connColor;

  signal(SIGINT, console::signalHandler);

  MatFileHandler mfh;

  std::string matStr;

  DfsColorGrid dfsCG(mfh);
  UnionFindColorGrid ufCG(mfh);

  parseCLIargs(argc, argv, matStr, algoChoice, mfh, paint, colors, crop, visEn, forceVis,
               connColor);

  ufCG.visualizerEn = visEn;
  ufCG.forceVis = forceVis;
  ufCG.connColor = connColor;

  algoNotifier(algoChoice);

  visNotifier(algoChoice, visEn);

  if (matStr.empty())
    processData(dfsCG, ufCG, mfh, algoChoice, paint, colors, crop);
  else {
    std::vector<Array2D<uint8_t>> matList;
    mfh.overwrite = true;
    matList = mfh.parseMatrices(matStr);
    for (Array2D<uint8_t> &mat : matList) {
      mfh.currMat = mat;
      mfh.parsedMatIdx++;
      timedSolver(dfsCG, ufCG, mfh.currMat, algoChoice, paint, colors, crop, mfh.filename);
      mfh.fWriteMat(mfh.filename, true);
    }
  }
  return 0;
}

void solver(DfsColorGrid &dfsCG, Array2D<uint8_t> &mat, bool &paint, bool &colors, bool crop,
            const std::string &filepath) {
  dfsCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
}

void solver(UnionFindColorGrid &ufCG, Array2D<uint8_t> &mat, bool &paint, bool &colors, bool crop,
            const std::string &filepath) {
  ufCG.calcMaxConnectedColor(mat, paint, colors, filepath, crop);
}

void processData(DfsColorGrid &dfsCG, UnionFindColorGrid &ufCG, MatFileHandler &mfh,
                 const std::string &algo, bool &paint, bool &colors, bool crop) {
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
