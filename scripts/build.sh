#!/bin/bash

# Usage: ./scripts/build.sh [--overwrite] [-j <jobs>] [--warnings] [--verbose]

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"

BUILD_DIR="${PROJECT_ROOT}/build"

# Defaults
JOBS_FLAG=""
OVERWRITE=0
SHOW_WARNINGS=0   # Default: warnings off
VERBOSE=0         # Default: no verbose output

# Help function
print_help() {
  echo "Usage: ./scripts/build.sh [options]"
  echo ""
  echo "Options:"
  echo "  --overwrite        Clean the build directory before building"
  echo "  -j <jobs>          Number of parallel jobs to use during compilation"
  echo "  --warnings         Enable compiler warnings"
  echo "  --verbose          Enable verbose output during build"
  echo "  -h, --help         Show this help message and exit"
  exit 0
}

# Parse arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    --overwrite)
      OVERWRITE=1
      shift
      ;;
    -j)
      if [[ -n "$2" && "$2" =~ ^[0-9]+$ ]]; then
        JOBS_FLAG="-j$2"
        shift 2
      else
        echo "[build.sh] Error: -j flag requires a numeric argument"
        exit 1
      fi
      ;;
    --warnings)
      SHOW_WARNINGS=1
      shift
      ;;
    --verbose)
      VERBOSE=1
      shift
      ;;
    -h|--help)
      print_help
      ;;
    *)
      echo "[build.sh] Unknown argument: $1"
      echo "Use --help to see available options."
      exit 1
      ;;
  esac
done

if [[ $OVERWRITE -eq 1 ]]; then
  echo "[build.sh] Overwrite flag passed, running cleanup.sh..."
  "${SCRIPT_DIR}/cleanup.sh"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

if [[ $VERBOSE -eq 1 ]]; then
  echo "[build.sh] Building with verbose output enabled."
  cmake --trace-expand ..
  make VERBOSE=1 ${JOBS_FLAG}
else
  if [[ $SHOW_WARNINGS -eq 1 ]]; then
    echo "[build.sh] Building with compiler warnings enabled."
    cmake ..
  else
    echo "[build.sh] Building with compiler warnings disabled."
    cmake -DCMAKE_CXX_FLAGS="-w" ..
  fi
  make ${JOBS_FLAG}
fi

make install
