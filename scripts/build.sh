#!/bin/bash

# Resolve absolute paths
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR=$(realpath "$SCRIPT_DIR/..")
BUILD_DIR="$BASE_DIR/build"
CLEANUP_SCRIPT="$SCRIPT_DIR/cleanup.sh"

# Default flags
OVERWRITE=false
JOBS_ARG="-j"  # Use all processors

# Help message
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  -o, --overwrite           Remove existing build directory before building"
    echo "  -j, --jobs <number>       Specify number of processors to use (default: all available)"
    echo "  -h, --help                Display this help message"
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -o|--overwrite)
            OVERWRITE=true
            shift
            ;;
        -j|--jobs)
            if [[ -n "$2" && "$2" != -* ]]; then
                JOBS_ARG="-j$2"
                shift 2
            else
                JOBS_ARG="-j"
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

# Optionally clean build
if [ "$OVERWRITE" = true ]; then
    echo "[build.sh] Cleaning previous build with: $CLEANUP_SCRIPT"
    "$CLEANUP_SCRIPT"
fi

# Create and enter build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Define local install prefix (project root)
LOCAL_INSTALL_PREFIX="$BASE_DIR"

echo "[build.sh] Running cmake with install prefix: $LOCAL_INSTALL_PREFIX"
cmake -DCMAKE_INSTALL_PREFIX="$LOCAL_INSTALL_PREFIX" "$BASE_DIR"

echo "[build.sh] Building with make $JOBS_ARG"
make $JOBS_ARG

echo "[build.sh] Installing locally to $LOCAL_INSTALL_PREFIX"
make install

echo "[build.sh] Build and install complete."
