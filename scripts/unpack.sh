#!/bin/bash

# Usage: ./unpack.sh [--input <file>] [--verbosity <level>] [--help]

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"
UNPACKER_EXECUTABLE="${PROJECT_ROOT}/bin/unpacker"
SETENV_SCRIPT="${SCRIPT_DIR}/setenv.sh"

# Source setenv.sh to configure environment variables
if [[ -f "${SETENV_SCRIPT}" ]]; then
  # shellcheck source=/dev/null
  source "${SETENV_SCRIPT}"
else
  echo "[unpack.sh] Warning: setenv.sh not found at ${SETENV_SCRIPT}, continuing without it."
fi

# Default values
INPUT_FILE="${PROJECT_ROOT}/data/run00001.mid"
VERBOSITY=1

print_help() {
  echo "Usage: ./unpack.sh [options]"
  echo ""
  echo "Options:"
  echo "  --input <file>       Input MIDAS file (default: ./data/run00001.mid)"
  echo "  --verbosity <level>  Verbosity level (default: 1)"
  echo "  -h, --help           Show this help message and exit"
  exit 0
}

# Parse arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    --input)
      INPUT_FILE="$2"
      shift 2
      ;;
    --verbosity)
      VERBOSITY="$2"
      shift 2
      ;;
    -h|--help)
      print_help
      ;;
    *)
      echo "[unpack.sh] Unknown argument: $1"
      echo "Use --help to see available options."
      exit 1
      ;;
  esac
done

# Run the unpacker
echo "[unpack.sh] Running unpacker..."
echo "  Input file:  ${INPUT_FILE}"
echo "  Verbosity:   ${VERBOSITY}"

"${UNPACKER_EXECUTABLE}" "${INPUT_FILE}" "${VERBOSITY}"
