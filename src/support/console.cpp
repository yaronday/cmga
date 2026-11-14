#include "console.hpp"

namespace console {

#ifdef _WIN32

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

void resetConsoleColor() { SetConsoleTextAttribute(handle, DEFAULT_COLOR); }

void formatTxt(const std::string &txt, const unsigned short &colorCode, const std::string &ending) {
  SetConsoleTextAttribute(handle, colorCode);
  std::cout << txt << ending;
  if (colorCode != DEFAULT_COLOR)
    resetConsoleColor();
}

#elif __APPLE__

void resetConsoleColor() {
  std::cout << MAC_COLOR_CODE_RST; // Reset on macOS (ANSI escape codes)
}

void formatTxt(const std::string &txt, const unsigned short &colorCode, const std::string &ending) {
  std::cout << COLOR_CODE_PREFIX << colorCode << "m" << txt << ending;
  if (colorCode != DEFAULT_COLOR)
    resetConsoleColor();
}

#endif

void signalHandler(int signal) {
  if (signal == SIGINT) {
    std::cout << ABORTED_ << "!\n\n";
    resetConsoleColor();
    exit(0);
  }
}

} // namespace console
