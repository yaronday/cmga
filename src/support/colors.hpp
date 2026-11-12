#pragma once

#ifdef _WIN32
// Windows color codes (based on standard 16-color palette)
const unsigned short BLACK = 0;
const unsigned short BLUE = 1;
const unsigned short GREEN = 2;
const unsigned short CYAN = 3;
const unsigned short RED = 4;
const unsigned short MAGENTA = 5;
const unsigned short YELLOW = 6;
const unsigned short WHITE = 7;
const unsigned short LIGHT_GREY = 8;
const unsigned short LIGHT_BLUE = 9;
const unsigned short LIGHT_GREEN = 10;
const unsigned short LIGHT_CYAN = 11;
const unsigned short LIGHT_RED = 12;
const unsigned short LIGHT_MAGENTA = 13;
const unsigned short LIGHT_YELLOW = 14;
const unsigned short BRIGHT_WHITE = 15;
const unsigned short DEFAULT_COLOR = WHITE;
#else
// ANSI escape codes for macOS (or Unix-like systems)
const unsigned short BLACK = 30;
const unsigned short BLUE = 34;
const unsigned short GREEN = 32;
const unsigned short CYAN = 36;
const unsigned short RED = 31;
const unsigned short MAGENTA = 35;
const unsigned short YELLOW = 33;
const unsigned short WHITE = 37;
const unsigned short LIGHT_GREY = 90;
const unsigned short LIGHT_BLUE = 94;
const unsigned short LIGHT_GREEN = 92;
const unsigned short LIGHT_CYAN = 96;
const unsigned short LIGHT_RED = 91;
const unsigned short LIGHT_MAGENTA = 95;
const unsigned short LIGHT_YELLOW = 93;
const unsigned short BRIGHT_WHITE = 97;
const unsigned short DEFAULT_COLOR = WHITE;
#endif

inline unsigned short getColorFromKey(int key) {
  switch (key) {
  case 1:
    return BLUE;
  case 2:
    return GREEN;
  case 3:
    return CYAN;
  case 4:
    return MAGENTA;
  case 5:
    return YELLOW;
  case 6:
    return LIGHT_RED;
  case 7:
    return LIGHT_GREEN;
  case 8:
    return LIGHT_CYAN;
  case 9:
    return LIGHT_MAGENTA;
  default:
    return WHITE;
  }
}

inline char getSymbolFromKey(int key) {
  switch (key) {
  case 1:
    return '*';
  case 2:
    return '@';
  case 3:
    return '^';
  case 4:
    return '>';
  case 5:
    return '<';
  case 6:
    return '$';
  case 7:
    return '"';
  case 8:
    return '&';
  case 9:
    return '%';
  default:
    return '#';
  }
}

// Utility function to return the Graphviz color name from a key
inline const char *getGraphvizColorFromKey(int key) {
  switch (key) {
  case 1:
    return "royalblue";
  case 2:
    return "lime";
  case 3:
    return "darkcyan";
  case 4:
    return "mediumvioletred";
  case 5:
    return "blueviolet";
  case 6:
    return "steelblue";
  case 7:
    return "lightgreen";
  case 8:
    return "teal";
  case 9:
    return "mediumturqoise";
  default:
    return "white";
  }
}

inline constexpr char MAC_COLOR_CODE_RST[] = "\033[0m";
inline constexpr char COLOR_CODE_PREFIX[] = "\033[";
