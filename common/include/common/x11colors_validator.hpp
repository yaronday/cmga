/* MIT License
 *
 * Copyright (c) 2026 Yaron Dayan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#define PICO_X11COLORS_VALIDATOR_VERSION_MAJOR 1
#define PICO_X11COLORS_VALIDATOR_VERSION_MINOR 0
#define PICO_X11COLORS_VALIDATOR_VERSION_PATCH 0

#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <string_view>

inline bool isValidX11Color(const std::string &input) {
  if (input.empty())
    return false;

  if (input[0] == '#') {
    std::string hexPart = input.substr(1);
    if (hexPart.size() == 3 || hexPart.size() == 4 || hexPart.size() == 6 || hexPart.size() == 8) {
      return std::all_of(hexPart.begin(), hexPart.end(),
                         [](unsigned char c) { return std::isxdigit(c); });
    }
    return false;
  }

  std::string colorName = input;
  std::transform(colorName.begin(), colorName.end(), colorName.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  // ref: https://graphviz.org/doc/info/colors.html

  constexpr std::array<std::string_view, 148> x11Colors = {"aliceblue",
                                                           "antiquewhite",
                                                           "aqua",
                                                           "aquamarine",
                                                           "azure",
                                                           "beige",
                                                           "bisque",
                                                           "black",
                                                           "blanchedalmond",
                                                           "blue",
                                                           "blueviolet",
                                                           "brown",
                                                           "burlywood",
                                                           "cadetblue",
                                                           "chartreuse",
                                                           "chocolate",
                                                           "coral",
                                                           "cornflowerblue",
                                                           "cornsilk",
                                                           "crimson",
                                                           "cyan",
                                                           "darkblue",
                                                           "darkcyan",
                                                           "darkgoldenrod",
                                                           "darkgray",
                                                           "darkgreen",
                                                           "darkgrey",
                                                           "darkkhaki",
                                                           "darkmagenta",
                                                           "darkolivegreen",
                                                           "darkorange",
                                                           "darkorchid",
                                                           "darkred",
                                                           "darksalmon",
                                                           "darkseagreen",
                                                           "darkslateblue",
                                                           "darkslategray",
                                                           "darkslategrey",
                                                           "darkturquoise",
                                                           "darkviolet",
                                                           "deeppink",
                                                           "deepskyblue",
                                                           "dimgray",
                                                           "dimgrey",
                                                           "dodgerblue",
                                                           "firebrick",
                                                           "floralwhite",
                                                           "forestgreen",
                                                           "fuchsia",
                                                           "gainsboro",
                                                           "ghostwhite",
                                                           "gold",
                                                           "goldenrod",
                                                           "gray",
                                                           "green",
                                                           "greenyellow",
                                                           "grey",
                                                           "honeydew",
                                                           "hotpink",
                                                           "indianred",
                                                           "indigo",
                                                           "ivory",
                                                           "khaki",
                                                           "lavender",
                                                           "lavenderblush",
                                                           "lawngreen",
                                                           "lemonchiffon",
                                                           "lightblue",
                                                           "lightcoral",
                                                           "lightcyan",
                                                           "lightgoldenrodyellow",
                                                           "lightgray",
                                                           "lightgreen",
                                                           "lightgrey",
                                                           "lightpink",
                                                           "lightsalmon",
                                                           "lightseagreen",
                                                           "lightskyblue",
                                                           "lightslategray",
                                                           "lightslategrey",
                                                           "lightsteelblue",
                                                           "lightyellow",
                                                           "lime",
                                                           "limegreen",
                                                           "linen",
                                                           "magenta",
                                                           "maroon",
                                                           "mediumaquamarine",
                                                           "mediumblue",
                                                           "mediumorchid",
                                                           "mediumpurple",
                                                           "mediumseagreen",
                                                           "mediumslateblue",
                                                           "mediumspringgreen",
                                                           "mediumturquoise",
                                                           "mediumvioletred",
                                                           "midnightblue",
                                                           "mintcream",
                                                           "mistyrose",
                                                           "moccasin",
                                                           "navajowhite",
                                                           "navy",
                                                           "oldlace",
                                                           "olive",
                                                           "olivedrab",
                                                           "orange",
                                                           "orangered",
                                                           "orchid",
                                                           "palegoldenrod",
                                                           "palegreen",
                                                           "paleturquoise",
                                                           "palevioletred",
                                                           "papayawhip",
                                                           "peachpuff",
                                                           "peru",
                                                           "pink",
                                                           "plum",
                                                           "powderblue",
                                                           "purple",
                                                           "rebeccapurple",
                                                           "red",
                                                           "rosybrown",
                                                           "royalblue",
                                                           "saddlebrown",
                                                           "salmon",
                                                           "sandybrown",
                                                           "seagreen",
                                                           "seashell",
                                                           "sienna",
                                                           "silver",
                                                           "skyblue",
                                                           "slateblue",
                                                           "slategray",
                                                           "slategrey",
                                                           "snow",
                                                           "springgreen",
                                                           "steelblue",
                                                           "tan",
                                                           "teal",
                                                           "thistle",
                                                           "tomato",
                                                           "turquoise",
                                                           "violet",
                                                           "wheat",
                                                           "white",
                                                           "whitesmoke",
                                                           "yellow",
                                                           "yellowgreen"};

  return binary_search(x11Colors.begin(), x11Colors.end(), std::string_view(colorName));
}