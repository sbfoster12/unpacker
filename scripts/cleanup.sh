#!/bin/bash

# Remove build artifacts and install outputs

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"

echo "[cleanup.sh] Cleaning build/, install/, and lib/..."

rm -rf "${PROJECT_ROOT}/build"
rm -rf "${PROJECT_ROOT}/install"
rm -rf "${PROJECT_ROOT}/lib"
