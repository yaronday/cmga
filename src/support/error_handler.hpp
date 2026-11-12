#pragma once

#include <string>

#include <optional>

#include <format>

#include "constants.hpp"
#include "error_consts.hpp"

namespace err {

inline static std::string withinRange(int minv, int maxv);

struct ErrorContext {
  std::optional<int> row;
  std::optional<int> col;
  std::optional<char> invalidChar;

  std::optional<int> invalidValue;

  std::optional<std::string> invalidStr;

  std::optional<std::string> argName;

  std::optional<int> minValue;
  std::optional<int> maxValue;

  std::optional<double> size;
  std::optional<double> sizeLimit;

  std::optional<std::string> exceptionMsg;

  // Default constructor
  ErrorContext() = default;

  // Constructor for INVALID_CHAR error
  ErrorContext(int r, int c, char ch) : invalidChar(ch), row(r), col(c) {}

  // Constructor for INVALID_RANGE error
  ErrorContext(int value, const std::string &arg, int min, int max)
      : invalidValue(value), argName(arg), minValue(min), maxValue(max) {}

  // Constructor for ROW_LEN_MISMATCH error
  ErrorContext(int r) : row(r) {}

  // constructor for MULTIDIGIT error
  // (basically the whole value could be evaluated, triggering an invalid range
  // error, but this way is simpler and faster)
  ErrorContext(int r, int c, int value) : row(r), col(c), invalidValue(value) {}

  // constructor for Arg releated or NOT_NUMERICAL error
  ErrorContext(const std::string &arg) : argName(arg) {}

  // constructor for filesize limit
  ErrorContext(double filesize, double maxSize) : size(filesize), sizeLimit(maxSize) {}

  // constructor for image format error
  ErrorContext(std::string value, const std::string &arg) : invalidStr(value), argName(arg) {}

  // Constructor for exceptions
  ErrorContext(const std::exception &ex) : exceptionMsg(ex.what()) {}
};

// Function to generate error messages based on error code and context
inline std::string getErrMsg(ErrCode code, const ErrorContext &context) {
  std::string msg;
  msg.reserve(64); // Avoid reallocations for typical error message lengths

  switch (code) {
  case ErrCode::INVALID_CHAR:
    msg = INV_CHAR_ERR;
    if (context.invalidChar && context.row && context.col) {
      msg += std::format("{}{}{}{}{}{}", CHAR_PREFIX, *context.invalidChar, CHAR_SUFFIX, AT_ROW,
                         *context.row, AT_COL, *context.col);
    } else if (context.invalidChar) {
      msg += std::format("{}{}{}", CHAR_PREFIX, *context.invalidChar, CHAR_SUFFIX);
    }
    break;

  case ErrCode::INVALID_RANGE:
    if (context.invalidValue && context.argName && context.minValue && context.maxValue) {
      msg = std::format("{} {} {}{}{}", *context.argName, *context.invalidValue, IS_INVALID_ERR,
                        VAL_EXP, err::withinRange(*context.minValue, *context.maxValue));
    } else {
      msg = INV_RANGE_ERR;
    }
    break;

  case ErrCode::ROW_LEN_MISMATCH:
    msg = ROWLEN_ERR;
    if (context.row)
      msg += std::format("{}{}", AT_ROW, *context.row);
    break;

  case ErrCode::INVALID_ALGO_SELECTION: // has a special handler inside
                                        // cli_utils
    msg = INV_ALGO_ERR;
    break;

  case ErrCode::WRONG_CONN_COLOR:
    msg = INV_CONN_COLOR_ERR;
    if (context.argName)
      msg += format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::MULTIDIGIT:
    msg = MULTIDIG_ERR;
    if (context.row && context.col && context.invalidValue) {
      msg += std::format("{}{}{}{}{}{}", AT_ROW, *context.row, AT_COL, *context.col, COLON_SP,
                         *context.invalidValue);
    }
    break;

  case ErrCode::UNEXPECTED_ARG:
    msg = UNEXP_ARG_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::MISSING_ARG_VALUE:
    msg = MISSING_ARGVAL_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::INT_EXPECTED:
    msg = INT_AFTER_ARG_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::NOT_NUMERICAL:
    msg = std::format("{}{}", context.argName ? *context.argName : VAL, NONNUMERIC_ERR);
    break;

  case ErrCode::EMPTY_STR:
    msg = EMPTYSTR_ERR;
    break;

  case ErrCode::INVALID_MAT:
    msg = INV_MAT_ERR;
    break;

  case ErrCode::INV_MAT_RANGE:
    msg = MAT_IDX_RANGE_ERR;
    break;

  case ErrCode::MAT_EMPTY:
    msg = MAT_EMPTY_ERR;
    break;

  case ErrCode::GEN_MATRIX_FAILED:
    msg = MAT_GEN_ERR;
    break;

  case ErrCode::NO_MAT_FOUND:
    msg = NO_MAT_FOUND_ERR;
    break;

  case ErrCode::UNKNOWN_OPTION:
    msg = UNKNOWN_OPT_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  // Graphviz related errors
  case ErrCode::GRAPHVIZ_NA:
    msg = GRAPHVIZ_NA_ERR;
    break;

  case ErrCode::GRAPHVIZ_MAT_LIMIT_REACHED:
    msg = std::format("{}{}", GRAPHVIZ_MAT_LIMIT, VIS_MAT_THR);
    break;

  // File related errors
  case ErrCode::FILE_OPEN_ERROR:
    msg = FILE_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::INVALID_DIR:
    msg = DIR_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::EMPTY_DIR:
    msg = EMPTYDIR_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::CDIR_FAILURE:
    msg = CDIR_ERR;
    if (context.argName)
      msg += std::format("{}{}", COLON_SP, *context.argName);
    break;

  case ErrCode::INVALID_FILE_EXT:
    msg = std::format("'{}'{}{}", context.argName ? *context.argName : "", COLON_SP,
                      INVALID_FEXT_ERR);
    break;

  case ErrCode::OVR_NOT_ENABLED:
    if (context.argName)
      msg = std::format("{}{}\n", FILE_, *context.argName);
    msg += WR_LOCK_ERR;
    break;

  case ErrCode::FILESIZE_LIMIT_REACHED:
    msg = FILESIZE_LIMIT_ERR;
    if (context.sizeLimit && context.size)
      msg += std::format("{}{}{}", *context.size, GREATER_THAN_, *context.sizeLimit);
    break;

  case ErrCode::GENERIC_EXCEPTION:
    msg = GENERIC_EXCEPTION_ERR;
    if (context.exceptionMsg) {
      msg += std::format("{}{}", COLON_SP, *context.exceptionMsg);
    }
    break;

  default:
    msg = UNKNOWN_ERR;
    break;
  }

  return std::format("{}!\n", msg);
}

// valid range notifier
inline static std::string withinRange(int minv, int maxv) {
  return std::format("within [{}, {}]", minv, maxv);
}

// Basic error handler (no parametrized context)
void handle(ErrCode code, const ErrorContext &context);

// INVALID_CHAR error handler
void handle(ErrCode code, char invalidChar, int row, int col);

// INVALID_CHAR wo specifying row and column
void handle(ErrCode code, char invalidChar);

// INVALID_RANGE handler
void handle(ErrCode code, int value, const std::string &arg, int min, int max);

// IMAGE_FORMAT_ERR handler
void handle(ErrCode code, const std::string &value, const std::string &arg);

// ROW_LEN_MISMATCH handler
void handle(ErrCode code, int row);

// MULTIDIGIT error handler
void handle(ErrCode code, int row, int col, int val);

// NOT_NUMERICAL or Arg related error handler
void handle(ErrCode code, const std::string &val);

// Filesize limit reached
void handle(ErrCode code, double size, double limit);

// Generic Exception handler
void handle(ErrCode code, const std::exception &ex);

} // namespace err