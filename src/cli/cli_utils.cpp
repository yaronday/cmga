#include "cli_utils.hpp"
#include "cli_consts.hpp"
#include "common/x11colors_validator.hpp"

void mainMenuDisplay() {
  title_and_ver();
  console::formatTxt("Main App:", LIGHT_CYAN);
  std::cout << std::format("Usage: cmga {} {} {} {} {} {} {} {} {} {}\n"
                           "             {} [{} <conn_color>] [{} <root_dir>] [{} "
                           "<algo>] [{} <matString>]\n",
                           P_HELP, P_H, P_H_WIN, P_VERSION, P_V, P_LIC, P_PAINT, P_NO_COLOR, P_CROP,
                           P_VISUALIZER, P_FORCE_VIS, P_CONN_COLOR, P_ROOTDIR, P_ALGO, P_MATRIX)
            << std::format("  {:<12} : Select graph algorithm: DFS, UF or BOTH.\n", P_ALGO)
            << std::format("  {:<12} : Paint max connected color regions.\n", P_PAINT)
            << std::format("  {:<12} : Paint black and white (using unique symbols).\n"
                           "                 If neither paint nor no_color arg is used, "
                           "then results will be saved under root_dir.\n",
                           P_NO_COLOR)
            << std::format("  {:<12} : The root and destination directory for processing "
                           "matrix files and storing results.\n",
                           P_ROOTDIR)
            << std::format(
                   "  {:<12} : Specify matrices (C/C++ std::format) as a std::string, e.g.\n"
                   "                 {} \"{}\"\n"
                   "                 Values allowed: integers {}.\n",
                   P_MATRIX, P_MATRIX, MATSTR, err::withinRange(MIN_V_LIMIT, MAX_V_LIMIT))
            << std::format("  {:<12} : Union-Find Roots visualizer. Limited by design "
                           "choice to "
                           "small matrices with max(rows, cols) <= {}).\n",
                           P_VISUALIZER, VIS_MAT_THR)
            << std::format("  {:<12} : Force visualizer even if matrix exceeds "
                           "threshold. Use with caution.\n"
                           "                 Graphviz must be installed and added to "
                           "System Env. Path.\n",
                           P_FORCE_VIS)
            << std::format("  {:<12} : Color of visualizer node connectors, specified by "
                           "X11 color std::string or hex code, e.g.\n"
                           "                --conn_color blue",
                           P_CONN_COLOR)
            << std::format("  {:<12} : Save max regions (inc. their original "
                           "coordinates) into files. Ignored if painting is used.\n",
                           P_CROP);
  console::formatTxt(
      std::format("  {} or {} or {} : Display this help menu.", P_HELP, P_H, P_H_WIN),
      LIGHT_MAGENTA);
  console::formatTxt(std::format("  {}       : Display license information.", P_LIC),
                     LIGHT_MAGENTA);
  console::formatTxt(std::format("  {} or {} : Display title and version.\n", P_VERSION, P_V),
                     LIGHT_MAGENTA);
  console::formatTxt("  Note: unlike in Filegen, overwrite is permanently enabled for "
                     "this operation.\n",
                     LIGHT_YELLOW);
  console::formatTxt(std::format("Example: cmga {} C:/CMGA/data/default_matrices {} {} dfs\n",
                                 P_ROOTDIR, P_PAINT, P_ALGO),
                     LIGHT_GREEN);
  console::formatTxt(std::format("Example: cmga {} data/multicolor {} {} {} both\n", P_ROOTDIR,
                                 P_VISUALIZER, P_CROP, P_ALGO),
                     LIGHT_GREEN);
  console::formatTxt(std::format("Example: ./cmga_mac {} data/big1 {}\n", P_ROOTDIR, P_FORCE_VIS),
                     LIGHT_GREEN);

  console::formatTxt("Array2D Files Generator:", LIGHT_CYAN);
  std::cout << std::format("Usage: cmga filegen {} {} {} [{} <rootDir>] {}\n"
                           "                    [{} <min_rows>] [{} <max_rows>] [{} "
                           "<row_inc>]\n"
                           "                    [{} <min_cols>] [{} <max_cols>] [{} "
                           "<col_inc>] \n"
                           "                    [{} <min_v>][{} <max_v>]\n",
                           P_SQUARE, P_CONFIRM, P_OVR, P_ROOTDIR, P_CSV, P_MINROWS, P_MAXROWS,
                           P_ROW_INC, P_MINCOLS, P_MAXCOLS, P_COL_INC, P_MINV, P_MAXV)
            << std::format("  {:<20} : Destination directory for storing the generated "
                           "files.\n",
                           P_ROOTDIR)
            << std::format("  {:<20} : Generate square matrix files, skipping "
                           "column-related parameters.\n",
                           P_SQUARE)
            << std::format("  {:<20} : Automatically confirm saving files to disk.\n", P_CONFIRM)
            << std::format("  {:<20} : Enable overwrite.\n", P_OVR)
            << std::format("  {:<20} : Save csv files (otherwise: txt, space-separated).\n", P_CSV)
            << std::format("  {} <min_rows>: min_rows <= {}.\n", P_MINROWS, ROWS_MAX_V)
            << std::format("  {} <max_rows>: max_rows <= {}.\n", P_MAXROWS, ROWS_MAX_V)
            << std::format("  {} <row_inc> : row_inc <= {}.\n", P_ROW_INC, INC_MAX)
            << std::format("  {} <min_cols>: min_cols <= {}.\n", P_MINCOLS, ROWS_MAX_V)
            << std::format("  {} <max_cols>: max_cols <= {}.\n", P_MAXCOLS, ROWS_MAX_V)
            << std::format("  {} <col_inc> : col_inc <= {}.\n", P_COL_INC, INC_MAX)
            << std::format("  {} <min_v>      : min_v {}.\n", P_MINV,
                           err::withinRange(MIN_V_LIMIT, MAX_V_LIMIT - 1))
            << std::format("  {} <max_v>      : max_v {}.\n", P_MAXV,
                           err::withinRange(MIN_V_LIMIT + 1, MAX_V_LIMIT));
  console::formatTxt(std::format("  Note: {} cannot be used with column parameters.\n", P_SQUARE),
                     LIGHT_YELLOW);
  console::formatTxt(
      std::format("Examples: cmga filegen {} 750 {} 400 {} 20\n", P_MAXROWS, P_MAXCOLS, P_COL_INC),
      LIGHT_GREEN);
  console::formatTxt(std::format("          cmga filegen {} {} 4000 {} 5000 {} 500 {} \n", P_SQUARE,
                                 P_MINROWS, P_MAXROWS, P_ROW_INC, P_CSV),
                     LIGHT_GREEN);
}

void handleArgs(int argc, char *argv[], std::string &matStr, std::string &algoChoice,
                MatFileHandler &mfh, bool &paint, bool &colors, bool &crop, bool &visEn,
                bool &forceVis, std::string &connColor) {

  int min_rows = MIN_ROW_V;
  int max_rows = MAX_ROW_V;
  int row_inc = INC_V;
  int min_cols = MIN_ROW_V;
  int max_cols = MAX_ROW_V;
  int col_inc = INC_V;
  int min_v = MIN_V;
  int max_v = MAX_V;

  bool filegen = FILEGEN_EN;

  bool algoSpecified = IS_ALGO_SPECIFIED;
  bool skip_algo_handler = SKIP_ALGO_HANDLER;

  forceVis = FORCE_VIS; // default value for force visualizer option
  connColor = CONN_COLOR;

  // Map to store flags and their values
  std::map<std::string, std::string> args_map;

  // filegen column parameter flags (to be banned in case of square matrix)
  std::unordered_set<std::string> fg_col_param_flags = {P_MINCOLS, P_MAXCOLS, P_COL_INC};

  std::unordered_set<std::string> param_flags = {
      P_ALGO, P_ROOTDIR, P_MATRIX, P_MINROWS, P_MAXROWS, P_ROW_INC, P_MINV, P_MAXV, P_CONN_COLOR};

  param_flags.insert(fg_col_param_flags.begin(), fg_col_param_flags.end());

  std::unordered_set<std::string> standalone_flags = {
      P_HELP, P_H,          P_H_WIN,     P_PAINT,  P_NO_COLOR, P_CSV, P_CROP,    P_FILEGEN,
      P_LIC,  P_VISUALIZER, P_FORCE_VIS, P_SQUARE, P_CONFIRM,  P_OVR, P_VERSION, P_V};

  // Parse the arguments and map them
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    std::ranges::transform(arg, arg.begin(), ::tolower);

    if (standalone_flags.contains(arg)) {
      args_map[arg] = "";
    } else if (param_flags.contains(arg)) {
      if (i + 1 < argc && !isFlag(argv[i + 1])) {
        args_map[arg] = argv[++i];
      } else {
        err::handle(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
      }
    } else {
      err::handle(ErrCode::UNKNOWN_OPTION, arg);
      cliErrHandler();
    }
  }

  // Validate and process each argument
  for (const auto &[fst, snd] : args_map) {
    const std::string &arg = fst;
    const std::string &value = snd;

    if (arg == P_HELP || arg == P_H || arg == P_H_WIN) {
      mainMenuDisplay();
      exit(0);
    }

    if (arg == P_LIC) {
      showLicense();
      exit(0);
    }

    if (arg == P_VERSION || arg == P_V) {
      title_and_ver();
      exit(0);
    }

    if (arg == P_PAINT) {
      paint = true;
    }

    if (arg == P_CONFIRM) {
      mfh.confirm = true;
    }

    if (arg == P_NO_COLOR) {
      paint = true;
      colors = false;
    }

    if (arg == P_CROP) {
      crop = true;
    }

    if (arg == P_CSV) {
      mfh.txt = false;
    }

    if (arg == P_ROOTDIR) {
      if (value.empty()) {
        err::handle(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
      }
      mfh.destpath = value;
    }

    if (arg == P_ALGO) {
      if (value.empty()) {
        err::handle(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
      }
      handleAlgoSelection(algoChoice, value);
      algoSpecified = true;
    }

    if (arg == P_MATRIX) {
      if (value.empty()) {
        err::handle(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
      }
      matStr = value;
    }

    if (arg == P_VISUALIZER || arg == P_FORCE_VIS) {
      if (!system(GRAPHVIZ_CHECK)) {
        visEn = true;
      } else {
        err::handle(ErrCode::GRAPHVIZ_NA, err::ErrorContext());
        cliErrHandler();
      }
    }

    if (arg == P_FORCE_VIS) {
      forceVis = true;
    }

    if (arg == P_CONN_COLOR) {
      if (value.empty()) {
        err::handle(ErrCode::MISSING_ARG_VALUE, arg);
        cliErrHandler();
      }
      if (isValidX11Color(value))
        connColor = value;
      else {
        err::handle(ErrCode::WRONG_CONN_COLOR, value);
        cliErrHandler();
      }
    }

    if (arg == P_FILEGEN) {
      filegen = true;
      skip_algo_handler = true;
    }

    if (arg == P_SQUARE) {
      mfh.sqmat = true;
    }

    if (arg == P_OVR) {
      mfh.overwrite = true;
    }

    if (arg == P_MINROWS) {
      auto it = args_map.find(P_MAXROWS);
      int maxRowsVal = (it == args_map.end() || it->second.empty()) ? max_rows : stoi(it->second);
      if (maxRowsVal > ROWS_MAX_V) {
        err::handle(ErrCode::INVALID_RANGE, maxRowsVal, arg, ROWS_MIN_V, ROWS_MAX_V);
        cliErrHandler();
      }
      validateAndAssign(min_rows, arg, ROWS_MIN_V, std::min(ROWS_MAX_V, maxRowsVal), args_map);
    }

    if (arg == P_MAXROWS) {
      auto it = args_map.find(P_MINROWS);
      int minRowsVal = (it == args_map.end() || it->second.empty()) ? min_rows : stoi(it->second);
      if (minRowsVal > ROWS_MAX_V) {
        err::handle(ErrCode::INVALID_RANGE, minRowsVal, arg, ROWS_MIN_V, ROWS_MAX_V);
        cliErrHandler();
      }
      validateAndAssign(max_rows, arg, std::max(minRowsVal, ROWS_MIN_V), ROWS_MAX_V, args_map);
    }

    if (arg == P_ROW_INC) {
      validateAndAssign(row_inc, arg, INC_MIN, INC_MAX, args_map);
    }

    if (arg == P_MINCOLS) {
      auto it = args_map.find(P_MAXCOLS);
      int maxColsVal = (it == args_map.end() || it->second.empty()) ? max_cols : stoi(it->second);
      if (maxColsVal > ROWS_MAX_V) {
        err::handle(ErrCode::INVALID_RANGE, maxColsVal, arg, ROWS_MIN_V, ROWS_MAX_V);
        cliErrHandler();
      }
      validateAndAssign(min_cols, arg, ROWS_MIN_V, std::min(ROWS_MAX_V, maxColsVal), args_map);
    }

    if (arg == P_MAXCOLS) {
      auto it = args_map.find(P_MINCOLS);
      int minColsVal = (it == args_map.end() || it->second.empty()) ? min_cols : stoi(it->second);
      if (minColsVal > ROWS_MAX_V) {
        err::handle(ErrCode::INVALID_RANGE, minColsVal, arg, ROWS_MIN_V, ROWS_MAX_V);
        cliErrHandler();
      }
      validateAndAssign(max_cols, arg, std::max(ROWS_MIN_V, minColsVal), ROWS_MAX_V, args_map);
    }

    if (arg == P_COL_INC) {
      validateAndAssign(col_inc, arg, INC_MIN, INC_MAX, args_map);
    }

    if (arg == P_MINV) {
      auto it = args_map.find(P_MAXV);
      int maxCellVal = (it == args_map.end() || it->second.empty()) ? max_v : stoi(it->second);
      maxCellVal = std::max(MIN_V + 1, std::min(MAX_V, maxCellVal)); // Clamp to valid maxv range
      validateAndAssign(min_v, arg, MIN_V, std::min(MAX_V - 1, maxCellVal - 1), args_map);
    }

    if (arg == P_MAXV) {
      auto it = args_map.find(P_MINV);
      int minCellVal = (it == args_map.end() || it->second.empty()) ? min_v : stoi(it->second);
      minCellVal = std::max(MIN_V, std::min(MAX_V - 1, minCellVal)); // Clamp to valid minv range
      validateAndAssign(max_v, arg, std::max(minCellVal + 1, MIN_V + 1), MAX_V, args_map);
    }
  }

  if (!algoSpecified && !skip_algo_handler)
    handleAlgoSelection(algoChoice);

  createDir(mfh.destpath);

  if (filegen) {

    mfh.updateMatParams(min_rows, max_rows, row_inc, min_cols, max_cols, col_inc, min_v, max_v);

    console::formatTxt(MATFILE_GEN_ACTIVATED, LIGHT_GREEN);

    if (mfh.sqmat) {
      for (const auto &col_flag : fg_col_param_flags) {
        if (!args_map[col_flag].empty()) {
          console::formatTxt(COL_SQMAT_ERROR, LIGHT_RED);
          exit(1);
        }
      }
    }

    mfh.invokeGenMatFiles();
    exit(0);
  }
}

void validateAndAssign(int &variable, const std::string &arg, int min_value, int max_value,
                       std::map<std::string, std::string> &args_map) {
  auto it = args_map.find(arg);
  if (it != args_map.end() && !it->second.empty()) {
    std::string st = it->second;
    if (utils::isNumeric(st)) {
      int value = stoi(st);
      if (value < min_value || value > max_value) {
        err::handle(ErrCode::INVALID_RANGE, value, arg, min_value, max_value);
        cliErrHandler();
      }
      variable = value;
    } else {
      err::handle(ErrCode::INT_EXPECTED, arg);
      cliErrHandler();
    }
  } else {
    err::handle(ErrCode::MISSING_ARG_VALUE, arg);
    cliErrHandler();
  }
}

void cliErrHandler(bool menu_disp) {
  if (menu_disp)
    mainMenuDisplay();
  exit(1);
}

bool isFlag(const std::string &str) {
  return str[0] == '-' && (str.length() > 1 && (str[1] == '-' || isalpha(str[1])));
}

bool isInvalidAlgoChoice(const std::string &algoChoice) {
  return (algoChoice != DFS_ && algoChoice != UF_ && algoChoice != BOTH_);
}

void handleAlgoSelection(std::string &algo, const std::string &value) {
  algo = utils::toUpper(value);
  if (isInvalidAlgoChoice(algo)) {
    err::handle(ErrCode::INVALID_ALGO_SELECTION, err::ErrorContext());
    cliErrHandler();
  }
}

void algoNotifier(const std::string &algo) {
  std::string algorithmText = (algo == BOTH_) ? ALGOS_WERE_ : ALGO_WAS_;
  console::formatTxt(std::format("{}{}{}!\n", algo, algorithmText, SELECTED), LIGHT_GREEN);
}

void visualizerNotifier(const std::string &algo, bool &visEnable) {
  if (visEnable) {
    if (algo != UF_ && algo != BOTH_) {
      // warn the user
      console::formatTxt(VIS_DISABLE, LIGHT_YELLOW);
      visEnable = false;
    }
  }
}

void title_and_ver() {
  console::formatTxt(std::format("{}{}{}", TITLE, VERSION, COPYRIGHT), LIGHT_CYAN);
}
