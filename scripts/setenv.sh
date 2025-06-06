#!/bin/bash

# Get absolute path to the root of the repo (assuming this script is in scripts/)
export UNPACKER_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$UNPACKER_PATH/include:$UNPACKER_PATH/external/midas:$(pkg-config --cflags nlohmann_json 2>/dev/null | sed 's/-I//g')
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$UNPACKER_PATH/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$UNPACKER_PATH/lib

echo "Environment configured from $UNPACKER_PATH"
