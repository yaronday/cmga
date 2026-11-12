#!/usr/bin/env bash
set -euo pipefail
find .. \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +