#!/usr/bin/env bash
set -e

# Colors for clarity
GREEN="\033[1;32m"
RESET="\033[0m"

BUILD_DIR="build"

# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
echo -e "${GREEN}> Building project...${RESET}"
mkdir -p $BUILD_DIR && cd $BUILD_DIR
cmake ..
make
cd ..

# -----------------------------------------------------------------------------
# Done
# -----------------------------------------------------------------------------
echo -e "${GREEN}> All builds completed successfully!${RESET}"
