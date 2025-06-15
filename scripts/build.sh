#!/bin/bash

# Resolve absolute paths
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR=$(realpath "$SCRIPT_DIR/..")
BUILD_DIR="$BASE_DIR/build"
SUBMODULE_BUILD_SCRIPT="$BASE_DIR/scripts/submodules/build_submodules.sh"
CLEANUP_SCRIPT="$SCRIPT_DIR/cleanup.sh"

# Default flags
OVERWRITE=false
RECURSIVE_OVERWRITE=false
JOBS_ARG="-j"  # Use all processors

# Help message
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  -o, --overwrite              Remove existing build directory before building"
    echo "  -r, --recursive-overwrite    Also clean and rebuild all submodules recursively"
    echo "  -j, --jobs <number>          Specify number of processors to use (default: all available)"
    echo "  -h, --help                   Display this help message"
}

# Parse arguments
SUBMODULE_ARGS=()
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -o|--overwrite)
            OVERWRITE=true
            shift
            ;;
        -r|--recursive-overwrite)
            RECURSIVE_OVERWRITE=true
            SUBMODULE_ARGS+=("--recursive-overwrite")
            shift
            ;;
        -j|--jobs)
            if [[ -n "$2" && "$2" != -* ]]; then
                JOBS_ARG="-j$2"
                SUBMODULE_ARGS+=("--jobs" "$2")
                shift 2
            else
                JOBS_ARG="-j"
                SUBMODULE_ARGS+=("--jobs")
                shift
            fi
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "[build.sh, ERROR] Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# If recursive overwrite is enabled, we also clean the current build
if [ "$RECURSIVE_OVERWRITE" = true ]; then
    OVERWRITE=true
fi

# Build submodules
echo "[build.sh] Invoking submodule build script: $SUBMODULE_BUILD_SCRIPT"
"$SUBMODULE_BUILD_SCRIPT" "${SUBMODULE_ARGS[@]}"

# Optionally clean current project build
if [ "$OVERWRITE" = true ]; then
    echo "[build.sh] Cleaning previous build with: $CLEANUP_SCRIPT"
    "$CLEANUP_SCRIPT"
fi

# Create and enter build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run CMake and Make
echo "[build.sh] Running cmake in: $BUILD_DIR"
cmake -DCMAKE_CXX_FLAGS="-Wno-cpp" "$BASE_DIR"

echo "[build.sh] Building with make $JOBS_ARG"
make $JOBS_ARG
make install

echo "[build.sh] Build complete."
echo "[build.sh] Executables are in: $BUILD_DIR/bin/"
echo "[build.sh] Libraries are in: $BUILD_DIR/lib/"
