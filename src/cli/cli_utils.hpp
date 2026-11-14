#pragma once

#include <map>
#include <unordered_set>             // IWYU pragma: keep
#include "analyzer/max_color_uf.hpp" // IWYU pragma: keep
#include "cli_consts.hpp"            // IWYU pragma: keep
#include "io/matfile_handler.hpp"
#include "support/error_handler.hpp"

const std::string algoSelect = ALGO_SELECTION;

void mainMenuDisplay();

void cliErrHandler(bool menu_disp = false);

void parseCLIargs(int argc, char *argv[], std::string &matStr, std::string &algoChoice,
                  MatFileHandler &mfh, bool &paint, bool &colors, bool &crop, bool &visEn,
                  bool &forceVis, std::string &connColor);

void validateAndAssign(int &variable, const std::string &arg, int min_value, int max_value,
                       std::map<std::string, std::string> &args_map);

bool isFlag(const std::string &);

bool isInvalidAlgoChoice(const std::string &algoChoice);

void algoNotifier(const std::string &algo);

void visNotifier(const std::string &algo, bool &visEnable);

void title_and_ver();

void handleAlgoSelection(std::string &algo, const std::string &value = UF_);

// clang-format off
inline void showLicense() {
  title_and_ver();
  console::formatTxt(
      "\n"
      "MIT License\n\n"
      "Copyright (c) 2026 Yaron Dayan\n\n"
      "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
      "of this software and associated documentation files (the 'Software'), to deal\n"
      "in the Software without restriction, including without limitation the rights\n"
      "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
      "copies of the Software, and to permit persons to whom the Software is\n"
      "furnished to do so, subject to the following conditions:\n\n"
      "The above copyright notice and this permission notice shall be included in all\n"
      "copies or substantial portions of the Software.\n\n"
      "THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
      "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
      "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
      "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
      "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
      "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
      "SOFTWARE.\n",
      LIGHT_CYAN);
}
// clang-format on