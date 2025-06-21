#!/bin/bash

# Resolve absolute paths
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR=$(realpath "$SCRIPT_DIR/..")
BUILD_DIR="$BASE_DIR/build"

# Help message
show_help() {
    echo "Usage: ./install.sh"
    echo
    echo "Installs the project to the system (default: /usr/local)."
    echo "This will delete the current build directory before configuring."
    echo "Note: This will likely require sudo."
}

# Handle help flag
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi

# Always delete previous build directory
echo "[install.sh] Cleaning previous build at: $BUILD_DIR"
rm -rf "$BUILD_DIR"

# Create fresh build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run CMake with default system install prefix
echo "[install.sh] Re-running CMake with system install prefix (/usr/local)..."
cmake "$BASE_DIR"

# Install with sudo
echo "[install.sh] Running 'sudo make install'..."
make -j
sudo make install

echo "[install.sh] Install complete to system default (/usr/local)"
