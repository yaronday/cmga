#pragma once

#include <iomanip>  // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <sstream>  // IWYU pragma: keep

#include <csignal>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#elif __APPLE__
#include <unistd.h> // For POSIX signal handling on macOS
#endif

#include "colors.hpp"
#include "constants.hpp" // IWYU pragma: keep

namespace console {

#ifdef _WIN32
extern HANDLE handle;
#endif

void resetConsoleColor();

void signalHandler(int);

void formatTxt(const std::string &txt, const unsigned short &colorCode = DEFAULT_COLOR,
               const std::string &ending = "\n");

} // namespace console
