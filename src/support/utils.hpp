#pragma once

#include <random>

#include <unordered_set>

#include "console.hpp" // IWYU pragma: keep

namespace utils {

std::string toUpper(std::string);

std::string toLower(std::string);

std::string removeSpaces(std::string_view);

void delimFix(std::string &line, const std::string &delimiters = ";,", const char &replacer = ' ');

std::vector<std::string> split(const std::string &);

bool isValidValue(const std::string &);

bool isNumeric(const std::string &);

int evalNum(size_t idx, std::vector<char> &buff);

bool isWithinRange(const int &, int, int);

std::vector<int> validateRow(const std::string &);

bool skipAction(const std::string &desc);

inline static char rndCharInRange(std::mt19937 &rng, char min_value, char max_value) {
  std::uniform_int_distribution<int> dist(min_value, max_value);
  return static_cast<char>('0' + dist(rng));
}

inline static int rndIntInRange(std::mt19937 &rng, char min_value, char max_value) {
  std::uniform_int_distribution<int> dist(min_value, max_value);
  return dist(rng);
}

inline char itoc(int num) { return static_cast<char>('0' + num); }

std::pair<int, int> adjustedRange(int start, int stop, int step);

} // namespace utils