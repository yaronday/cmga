#!/usr/bin/env bash

set -euo pipefail

FILTER="_out_max"

# Resolve project root relative to the script location (scripts/.. -> root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Target directory defaults to project root if not explicitly provided
TARGET_DIR="${1:-$PROJECT_ROOT/data}"

echo "Scanning [$TARGET_DIR] for files matching '*$FILTER*'."

# Find and remove files safely
find "$TARGET_DIR" -type f -name "*$FILTER*" -print -exec rm -f {} +

echo "Cleanup complete."