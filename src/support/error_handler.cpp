#include "error_handler.hpp"
#include "console.hpp"

namespace err {

// Basic error handler
void handle(ErrCode code, const ErrorContext &context) {
  console::formatTxt(getErrMsg(code, context), LIGHT_RED);
}

// INVALID_CHAR error handler
void handle(ErrCode code, char invalidChar, int row, int col) {
  ErrorContext context(row, col, invalidChar);
  handle(code, context);
}

// INVALID_CHAR wo specifying row and column
void handle(ErrCode code, char invalidChar) {
  // Create context with only the invalidChar
  ErrorContext context(invalidChar);
  handle(code, context);
}

// INVALID_RANGE handler
void handle(ErrCode code, int value, const std::string &arg, int min, int max) {
  ErrorContext context(value, arg, min, max);
  handle(code, context);
}

// IMAGE_FORMAT_ERR handler
void handle(ErrCode code, const std::string &value, const std::string &arg) {
  ErrorContext context(value, arg);
  handle(code, context);
}

// ROW_LEN_MISMATCH handler
void handle(ErrCode code, int row) {
  ErrorContext context(row);
  handle(code, context);
}

// MULTIDIGIT error handler
void handle(ErrCode code, int row, int col, int val) {
  ErrorContext context(row, col, val);
  handle(code, context);
}

// NOT_NUMERICAL or ARG related error handler
void handle(ErrCode code, const std::string &val) {
  ErrorContext context(val);
  handle(code, context);
}

// Filesize limit reached
void handle(ErrCode code, double size, double limit) {
  ErrorContext context(size, limit);
  handle(code, context);
}

// Generic Exception handler
void handle(ErrCode code, const std::exception &ex) {
  ErrorContext context(ex);
  handle(code, context);
}

} // namespace err