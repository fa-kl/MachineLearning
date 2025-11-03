#!/usr/bin/env bash
set -e

# Colors for clarity
GREEN="\033[1;32m"
RESET="\033[0m"

BUILD_DIR="build"

echo -e "${GREEN}> Cleaning old build...${RESET}"
if test -d "$BUILD_DIR"; then
    rm -rf "$BUILD_DIR"
fi

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
bash build.sh
