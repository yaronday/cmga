SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"

CONFIG_PRESET="default"
BUILD_TYPE='Release'

pause_on_windows() {
  if [[ "$WIN" == 'true' ]]; then
    read -p "${1:-Press Enter to exit...}"
  fi
}

OS=$(uname -s)
WINSTR='MINGW64_NT-10.0*'
PROJ_NAME='cmga'
TARGET_PATH="${PROJ_NAME}_local"

WIN=$([[ "$OS" == $WINSTR ]] && echo 'true' || echo 'false')

if [[ "$WIN" == 'true' ]]; then
  TARGET_DIR="$(cygpath -u "$USERPROFILE")/$TARGET_PATH"
  PROJ_NAME="${PROJ_NAME}.exe"
else
  TARGET_DIR="$HOME/$TARGET_PATH"
fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    --help | -h | "/?" )
      echo "Usage: $0 [--build <build_type>] [--clean] [target_dir]"
      echo "  --clean      Cleans build artifacts and static binaries, then builds (defaults to Release)."
      echo "  --build      Specify build type: Release, Debug, RelWithDebInfo, MinSizeRel"
      exit 0
      ;;
    --clean)
      echo "Cleaning build artifacts and static binaries..."
      [[ -d "$BUILD_DIR" ]] && rm -rf "$BUILD_DIR"
	  shift
      ;;
    --build)
      BUILD_TYPE="$2"
      shift 2
      ;;
    -*)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
    *)
      TARGET_DIR="$1"
      shift
      ;;
  esac
done

cd "$PROJECT_ROOT" || { echo "Failed to change directory to project root" >&2; exit 1; }

echo "Configuring preset: $CONFIG_PRESET..."
cmake --preset "$CONFIG_PRESET"
if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake configuration preset failed." >&2
  pause_on_windows
  exit 1
fi

echo "Building configuration: $BUILD_TYPE..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE"

if [[ "$?" -ne 0 ]]; then
  echo "Error: CMake build failed." >&2
  pause_on_windows
  exit 1
fi

EXE_PATH="$BUILD_DIR/bin/$BUILD_TYPE/$PROJ_NAME"

if [[ -f "$EXE_PATH" ]]; then  
  mkdir -p "$TARGET_DIR"
  cp "$EXE_PATH" "$TARGET_DIR/$PROJ_NAME"
else
  echo "Error: Executable not found at expected path: $EXE_PATH" >&2
  pause_on_windows
  exit 1
fi

echo "Done."
pause_on_windows