#pragma once

// Title & Version
inline constexpr char COPYRIGHT[] = "\nCopyright (C) Yaron Dayan 2026\n";
inline constexpr char VERSION[] = "v1.1.1";
inline constexpr char TITLE[] = "Color Matrix Genalyzer (CMGA) ";

// Matrix String
inline constexpr char MATSTR[] =
    R"({{1, 2, 1}, {2, 3, 2}, {1, 2, 1}}, {{1, 2, 1}, {2, 1, 2}, {1, 2, 3}})";

// io
inline constexpr char ROOTDIR[] = "data/multicolor";
inline constexpr bool EN_TXT = true;
inline constexpr bool EN_OVR = false;
inline constexpr bool EN_CONFIRM = false;
inline constexpr int TABWIDTH = 4;
inline constexpr int EXT_LEN = 3; // default file extension length
inline constexpr char DEFAULT_INFO[] = "file";
inline constexpr char FILENAME_FILTER[] = "out";
inline constexpr char OVR_[] = "Overwriting";
inline constexpr char WR_[] = "Writing";

// mat_files defaults
inline constexpr int MIN_ROW_V = 10;
inline constexpr int MAX_ROW_V = 20;
inline constexpr int INC_V = 10;
inline constexpr int MIN_V = 1;
inline constexpr int MAX_V = 3;
inline constexpr bool SQMAT = false;
inline constexpr char LOAD_MATRICES[] = "Loading matrix files...\n";
inline constexpr char LOAD_MAT_DONE[] = "\nLoading - Complete!\n\n";
inline constexpr char MATFILE_[] = "Matrix File ";
inline constexpr char COL_SQMAT_ERROR[] = "Column params cannot be used with --square!";
inline constexpr char OUT_MAX[] = "_out_max";
inline constexpr char CROPPED_[] = "_cropped_";
inline constexpr char COLOR_[] = "_color";
inline constexpr char TL_PREF[] = "_TL";
inline constexpr char TOPLEFT_[] = "Top Left: ";
inline constexpr char BOTTOMRIGHT_[] = "Bottom Right: ";
inline constexpr char BR_PREF[] = "_BR";
inline constexpr char EXTRA_INFO[] = "_extra_info";
inline constexpr char MAX_CON_COLORS[] = "Max connected colors ";
inline constexpr char RECT_BOUNDARIES[] = "\nRectangular boundaries for ";
inline constexpr char CROP_COORDINATES[] =
    " cropped max region (coordinates of original matrix):\n";
inline constexpr char CODES_W_MAX_SIZE[] = "Color codes with max size of ";
inline constexpr char COLOR_CODE_TXT[] = "Color code ";
inline constexpr char MULTIPLE_REGIONS[] = " (multiple regions)\n";
inline constexpr char F_MAXCOLOR_PRFIX[] = "_c";
inline constexpr char DATAPROC[] = "Processing data...\n";

// File Generator defaults; same init values for cols
inline constexpr int ROWS_MIN_V = 1;
inline constexpr int ROWS_MAX_V = 15000;
inline constexpr int INC_MIN = 1;
inline constexpr int INC_MAX = ROWS_MAX_V - 1;
inline constexpr int MIN_V_LIMIT = 1;
inline constexpr int MAX_V_LIMIT = 9;
inline constexpr double MAX_FILESIZE_MB = 500.0;
inline constexpr bool FILEGEN_EN = false;

// File Generator
inline constexpr double KB_SIZE = 1024.0;
inline constexpr char GB_UNIT[] = " GB";
inline constexpr char MB_UNIT[] = " MB";
inline constexpr char KB_UNIT[] = " KB";
inline constexpr char BYTES_UNIT[] = " Bytes";
inline constexpr char FILEGEN_[] = "files generation";
inline constexpr char MATFILE_GEN_ACTIVATED[] = "Matrix Files Generator activated!";
inline constexpr char REQ_SPACE[] = "Required space = ";
inline constexpr char APPROVED_[] = "OK!";
inline constexpr char SELECTED[] = " selected";
inline constexpr char GEN_RAND[] = "Generating a random ";
inline constexpr char MAT_WITH_VALS[] = " matrix with values ";
inline constexpr char ABORTED_[] = "Aborted by user";
inline constexpr char EXITING_[] = "Exiting...";
inline constexpr char CONFIRM_[] = " [Y / N] = ? ";

// Algo
inline constexpr bool IS_ALGO_SPECIFIED = false;
inline constexpr bool SKIP_ALGO_HANDLER = false;
inline constexpr char ALGO_SELECTION[] =
    "Select the algorithm to use: 'DFS', 'UF', 'BOTH' (default: BOTH).\n";
inline constexpr char DFS_[] = "DFS";
inline constexpr char UF_[] = "UF";
inline constexpr char BOTH_[] = "BOTH";
inline constexpr char ALGOS_WERE_[] = " algorithms were";
inline constexpr char ALGO_WAS_[] = " algorithm was";

// matrix content
inline constexpr char CHAR_PREFIX[] = " '";
inline constexpr char CHAR_SUFFIX[] = "'";
inline constexpr char AT_ROW[] = " at row ";
inline constexpr char AT_COL[] = ", col ";
inline constexpr char COLON_SP[] = ": ";
inline constexpr char VAL[] = "Value";
inline constexpr char VAL_EXP[] = "Value must be ";
inline constexpr char FILE_[] = "File: ";
inline constexpr char GREATER_THAN_[] = " > ";
inline constexpr char ARROW_[] = " --> ";

// visualizer config
inline constexpr int VIS_MAT_THR = 70;
inline constexpr char TREEPACK_THR = 40; // Node tree packing threshold
inline constexpr bool FORCE_VIS = false;
inline constexpr char VIS_DISABLE[] = "Disabling Visualizer since UF was not included.\n";
inline constexpr char GRAPHVIZ_CFG[] = "graph [layout=circo, pack=true, packmode=";
inline constexpr char SHORT_ARROW_[] = " -> ";
inline constexpr char FOUR_SPACES[] = "    ";
inline constexpr char CONN_COLOR[] = "black";
inline constexpr char DOTFILEOPENER[] = "{\n";
inline constexpr char DOTFILECLOSER[] = "}\n";
inline constexpr char SVG_[] = "svg";
inline constexpr char DOT_[] = "dot";
#if defined(_WIN32) || defined(_WIN64)
inline constexpr char GRAPHVIZ_CHECK[] = "dot -V >NUL 2>&1";
#else
inline constexpr char GRAPHVIZ_CHECK[] = "dot -V >/dev/null 2>&1";
#endif

inline constexpr char FONT_NAME[] = "Arial";
inline constexpr char FONT_COLOR[] = "black";
// unique root node settings
inline constexpr double R_FONT_SIZE = 16.00;
inline constexpr char R_STROKE_COLOR[] = "cyan";
