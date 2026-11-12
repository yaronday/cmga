#pragma once

// Enum for error codes
enum class ErrCode {
  // CLI related
  INVALID_ALGO_SELECTION,
  MISSING_ARG_VALUE,
  INT_EXPECTED,
  UNKNOWN_OPTION,
  UNEXPECTED_ARG,

  // Array2D related
  INVALID_MAT,
  ROW_LEN_MISMATCH,
  INV_MAT_RANGE,
  MAT_EMPTY,
  NO_MAT_FOUND,
  INVALID_RANGE,
  NOT_NUMERICAL,
  EMPTY_STR,
  INVALID_CHAR,
  MULTIDIGIT,
  GEN_MATRIX_FAILED,

  // File related
  FILE_OPEN_ERROR,
  INVALID_DIR,
  CDIR_FAILURE,
  INVALID_FILE_EXT,
  EMPTY_DIR,
  OVR_NOT_ENABLED,
  FILESIZE_LIMIT_REACHED,

  // Visualizer
  GRAPHVIZ_NA,
  GRAPHVIZ_MAT_LIMIT_REACHED,
  WRONG_CONN_COLOR,

  GENERIC_EXCEPTION
};

inline constexpr char IS_INVALID_ERR[] = " is invalid! ";

inline constexpr char ROOTDIR_ERR[] = "Invalid root directory";
inline constexpr char DIR_ERR[] = "Invalid directory";
inline constexpr char CDIR_ERR[] = "Failed to create directory";

inline constexpr char FILESIZE_LIMIT_ERR[] =
    "Estimated filesize[MB] required exceeds maximum limit";
inline constexpr char INVALID_FEXT_ERR[] = "Only 'txt' and 'csv' are supported";
inline constexpr char EMPTYDIR_ERR[] = "No matching files were found in directory";
inline constexpr char WR_LOCK_ERR[] = "add --ovr to modify existing file.";
inline constexpr char FILE_ERR[] = "Error opening file";

inline constexpr char INV_ALGO_ERR[] =
    "Invalid value for --algo option. Must be 'DFS', 'UF', or 'BOTH'";

inline constexpr char MAT_EMPTY_ERR[] = "Array2D is empty";
inline constexpr char INV_MAT_ERR[] = "Invalid Array2D";
inline constexpr char MAT_GEN_ERR[] = "Array2D generation failed";
inline constexpr char NO_MAT_FOUND_ERR[] = "No matrix was found";
inline constexpr char EMPTYSTR_ERR[] = "Empty string";
inline constexpr char MAT_IDX_RANGE_ERR[] = "Array2D index out of range";

inline constexpr char ROWLEN_ERR[] = "Row length mismatch";
inline constexpr char MULTIDIG_ERR[] = "Multidigit detected";
inline constexpr char NONNUMERIC_ERR[] = " is either not numerical or of an invalid format";
inline constexpr char INV_CHAR_ERR[] = "Invalid character";
inline constexpr char INV_RANGE_ERR[] = "Invalid range";

inline constexpr char GRAPHVIZ_NA_ERR[] =
    "The visualizer must have Graphviz installed and configured as system env "
    "variable";
inline constexpr char GRAPHVIZ_MAT_LIMIT[] = "Visualizer deactivated: Array2D max(m, n) > ";
inline constexpr char RENDER_CMD_FAILED[] = "Render command failed - \n";
inline constexpr char INV_CONN_COLOR_ERR[] = "Invalid connector color";

inline constexpr char UNEXP_ARG_ERR[] = "Unexpected Argument";
inline constexpr char MISSING_ARGVAL_ERR[] = "Missing value for argument";
inline constexpr char INT_AFTER_ARG_ERR[] = "There must be a positive integer after argument";

inline constexpr char UNKNOWN_OPT_ERR[] = "Unknown option";
inline constexpr char UNKNOWN_ERR[] = "Unknown error";
inline constexpr char GENERIC_EXCEPTION_ERR[] = "An exception occurred";