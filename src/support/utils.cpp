#include "utils.hpp"
#include "console.hpp"
#include "constants.hpp"
#include "error_handler.hpp"

namespace utils {

std::string toUpper(std::string s) {
  for (char &c : s) {
    c = toupper(static_cast<unsigned char>(c));
  }
  return s;
}

std::string toLower(std::string s) {
  for (char &c : s) {
    c = tolower(static_cast<unsigned char>(c));
  }
  return s;
}

std::string removeSpaces(std::string_view input) {
  std::string result;
  result.reserve(input.size());

  for (char ch : input) {
    if (!isspace(static_cast<unsigned char>(ch))) {
      result += ch;
    }
  }
  return result;
}

void delimFix(std::string &line, const std::string &delimiters, const char &replacer) {
  // Trim leading and trailing whitespace
  line.erase(0, line.find_first_not_of(" \t"));
  line.erase(line.find_last_not_of(" \t") + 1);

  // Replace delimiters with replacer char
  for (char &ch : line) {
    if (delimiters.find(ch) != std::string::npos) {
      ch = replacer;
    }
  }
}

std::vector<std::string> split(const std::string &input) {
  std::vector<std::string> substrings;
  size_t pos = 0;

  std::string temp = removeSpaces(input);

  size_t tempLen = temp.length();

  if (tempLen < 3) {
    err::handle(ErrCode::EMPTY_STR, err::ErrorContext());
    return {" "};
  }

  // Remove leading and trailing curly braces
  std::string trimmedStr = temp.substr(1, tempLen - 2);

  const std::string delimiter = "}},";

  size_t delimLen = delimiter.length();

  while ((pos = trimmedStr.find(delimiter, pos)) != std::string::npos) {
    substrings.emplace_back(trimmedStr.substr(0, pos + delimLen - 1));
    pos += delimLen;
    temp = trimmedStr.substr(pos);
    trimmedStr = temp;
    pos = 0;
  }

  // Add the last substring
  substrings.emplace_back(temp);

  return substrings;
}

bool isValidValue(const std::string &str) {
  if (str.empty()) {
    return false;
  }
  if (!isNumeric(str))
    return false;
  if (!isWithinRange(stoi(str), MIN_V_LIMIT, MAX_V_LIMIT))
    return false;
  return true;
}

bool isWithinRange(const int &value, int min_val, int max_val) {
  if ((value < min_val) || (value > max_val)) {
    err::handle(ErrCode::INVALID_RANGE, value, ARROW_, min_val, max_val);
    return false;
  }
  return true;
}

bool isNumeric(const std::string &str) {
  if (str.empty()) {
    err::handle(ErrCode::EMPTY_STR, err::ErrorContext());
    return false; // Early exit for empty strings
  }

  auto it = str.begin(); // Iterator to the first character

  // skip minus sign at the beginning
  if (*it == '-') {
    if (str.size() == 1) {
      err::handle(ErrCode::NOT_NUMERICAL, str);
      return false; // Just "-" is not valid
    }
    ++it; // Skip the minus sign
  }

  // Check if all remaining characters are digits
  for (char ch : std::string(it, str.end())) {
    if (!isdigit(ch)) {
      err::handle(ErrCode::NOT_NUMERICAL, str);
      return false; // Early exit on first invalid character
    }
  }
  return true; // All characters are digits
}

int evalNum(size_t idx, std::vector<char> &buff) {
  int res = 0;
  while (idx < buff.size() && isdigit(buff[idx])) {
    res = res * 10 + (buff[idx] - '0');
    ++idx;
  }
  return res;
}

std::vector<int> validateRow(const std::string &line) {
  std::vector<int> row;
  std::istringstream iss(line);
  std::string token;

  // Single pass: extract, validate, and parse simultaneously
  while (iss >> token) {
    if (!isValidValue(token)) {
      return {-1};
    }
    row.emplace_back(stoi(token));
  }

  return row;
}

bool skipAction(const std::string &desc) {
  console::formatTxt(format("Proceed with {}{}", desc, CONFIRM_), LIGHT_CYAN);
  char input;
  std::cin >> input;
  if (input == 'Y' || input == 'y') {
    console::formatTxt(APPROVED_, LIGHT_GREEN);
    return false;
  } else {
    console::formatTxt(EXITING_, LIGHT_CYAN);
    return true;
  }
}

// adjust end value and num of elements of a non-evenly spaced range
std::pair<int, int> adjustedRange(int start, int stop, int step) {
  std::pair<int, int> result;
  result.first = start + ((stop - start) / step) * step; // adjusted end values
  result.second = (result.first - start) / step + 1;     // num of elements
  return result;
}

} // namespace utils