#!/usr/bin/env bash
set -e

# Colors for clarity
GREEN="\033[1;32m"
BLUE="\033[1;34m"
YELLOW="\033[1;33m"
RED="\033[1;31m"
RESET="\033[0m"

echo -e "${BLUE}============================================${RESET}"
echo -e "${BLUE}  MNIST & EMNIST Dataset Preparation Tool  ${RESET}"
echo -e "${BLUE}============================================${RESET}"

# Create data directory structure if it doesn't exist
DATA_DIR="data"
MNIST_DIR="$DATA_DIR/mnist"
EMNIST_DIR="$DATA_DIR/emnist"

if [ ! -d "$DATA_DIR" ]; then
    echo -e "${GREEN}> Creating data directory structure...${RESET}"
    mkdir -p "$MNIST_DIR"
    mkdir -p "$EMNIST_DIR"
else
    mkdir -p "$MNIST_DIR"
    mkdir -p "$EMNIST_DIR"
fi

# ==============================================
# MNIST Dataset (Handwritten Digits 0-9)
# ==============================================
echo -e "\n${YELLOW}=== Downloading MNIST Dataset ===${RESET}"

MNIST_FILES=(
    "train-images-idx3-ubyte.gz"
    "train-labels-idx1-ubyte.gz"
    "t10k-images-idx3-ubyte.gz"
    "t10k-labels-idx1-ubyte.gz"
)

MNIST_URLS=(
    "https://github.com/golbin/TensorFlow-MNIST/raw/master/mnist/data/train-images-idx3-ubyte.gz"
    "https://github.com/golbin/TensorFlow-MNIST/raw/master/mnist/data/train-labels-idx1-ubyte.gz"
    "https://github.com/golbin/TensorFlow-MNIST/raw/master/mnist/data/t10k-images-idx3-ubyte.gz"
    "https://github.com/golbin/TensorFlow-MNIST/raw/master/mnist/data/t10k-labels-idx1-ubyte.gz"
)

MNIST_DESCRIPTIONS=(
    "MNIST Training Images (60,000 samples)"
    "MNIST Training Labels"
    "MNIST Test Images (10,000 samples)"
    "MNIST Test Labels"
)

# Change to MNIST directory for downloads
cd "$MNIST_DIR"

for i in "${!MNIST_FILES[@]}"; do
    file="${MNIST_FILES[$i]}"
    url="${MNIST_URLS[$i]}"
    desc="${MNIST_DESCRIPTIONS[$i]}"
    
    if [ -f "${file%.gz}" ]; then
        echo -e "${GREEN}✓ ${desc} already exists${RESET}"
    else
        echo -e "${BLUE}> Downloading ${desc}...${RESET}"
        if wget -q --show-progress "$url" -O "$file"; then
            echo -e "${GREEN}✓ Downloaded ${file}${RESET}"
        else
            echo -e "${RED}✗ Failed to download ${file}${RESET}"
            exit 1
        fi
    fi
done

# Go back to project root for EMNIST processing
cd "../.."

# ==============================================
# EMNIST Dataset (Extended MNIST - Letters)
# ==============================================
echo -e "\n${YELLOW}=== Downloading EMNIST Dataset ===${RESET}"

# EMNIST dataset - try multiple sources
EMNIST_SUCCESS=false

# Define EMNIST sources
EMNIST_SOURCES=(
    "https://biometrics.nist.gov/cs_links/EMNIST/gzip.zip|emnist-gzip.zip"
    "https://www.itl.nist.gov/iaui/vip/cs_links/EMNIST/gzip.zip|emnist-gzip.zip"
    "https://github.com/sorki/python-mnist/releases/download/v0.7/emnist-gzip.zip|emnist-gzip.zip"
)

# Change to EMNIST directory for downloads
cd "$EMNIST_DIR"

# Check if EMNIST is already downloaded and extracted
if [ "$(ls -A . 2>/dev/null)" ] && [ "$(find . -name "*ubyte" | wc -l)" -gt 0 ]; then
    echo -e "${GREEN}✓ EMNIST dataset already exists and extracted${RESET}"
    EMNIST_SUCCESS=true
    
    # Organize files if needed
    if [ -d "gzip" ]; then
        echo -e "${BLUE}  Organizing EMNIST files...${RESET}"
        mv gzip/* . 2>/dev/null || true
        rmdir gzip 2>/dev/null || true
    fi
else
    echo -e "${BLUE}> Downloading EMNIST dataset (Letters + Digits)...${RESET}"
    echo -e "${BLUE}  This may take a while (~300-500MB)...${RESET}"
    
    for source in "${EMNIST_SOURCES[@]}"; do
        IFS='|' read -r url filename <<< "$source"
        echo -e "${BLUE}  Trying source: $(basename "$url")${RESET}"
        
        # Clean up any partial downloads
        [ -f "$filename" ] && rm -f "$filename"
        
        if wget -q --show-progress --timeout=30 --tries=2 "$url" -O "$filename"; then
            # Verify the download
            if [ -f "$filename" ] && [ "$(stat -f%z "$filename" 2>/dev/null || stat -c%s "$filename" 2>/dev/null)" -gt 1000000 ]; then
                echo -e "${GREEN}✓ Successfully downloaded EMNIST from $(basename "$url")${RESET}"
                EMNIST_SUCCESS=true
                break
            else
                echo -e "${YELLOW}  Download appears incomplete, trying next source...${RESET}"
                [ -f "$filename" ] && rm -f "$filename"
            fi
        else
            echo -e "${YELLOW}  Failed to download from $(basename "$url"), trying next source...${RESET}"
        fi
    done
    
    if [ "$EMNIST_SUCCESS" = false ]; then
        echo -e "${YELLOW}  Note: EMNIST download failed from all sources. Continuing with MNIST only.${RESET}"
        echo -e "${YELLOW}  You can manually download EMNIST later from: https://www.nist.gov/itl/products-and-services/emnist-dataset${RESET}"
    fi
fi

# Go back to project root
cd "../.."

# ==============================================
# Extract all compressed files
# ==============================================
echo -e "\n${YELLOW}=== Extracting datasets ===${RESET}"

# Extract MNIST files
echo -e "${BLUE}> Extracting MNIST files...${RESET}"
cd "$MNIST_DIR"
for file in "${MNIST_FILES[@]}"; do
    if [ -f "$file" ] && [ ! -f "${file%.gz}" ]; then
        echo -e "  Extracting $file..."
        gunzip "$file"
        echo -e "${GREEN}  ✓ Extracted ${file%.gz}${RESET}"
    fi
done
cd "../.."

# Extract EMNIST files if available
if [ "$EMNIST_SUCCESS" = true ]; then
    echo -e "${BLUE}> Extracting EMNIST files...${RESET}"
    cd "$EMNIST_DIR"
    
    # Find the downloaded EMNIST file
    EMNIST_FILE=""
    for file in emnist-*.zip emnist-gzip.zip; do
        if [ -f "$file" ]; then
            EMNIST_FILE="$file"
            break
        fi
    done
    
    if [ -n "$EMNIST_FILE" ]; then
        if command -v unzip >/dev/null 2>&1; then
            # Test the zip file first
            if unzip -t "$EMNIST_FILE" >/dev/null 2>&1; then
                echo -e "${BLUE}  ZIP file is valid, extracting...${RESET}"
                if unzip -q "$EMNIST_FILE"; then
                    echo -e "${GREEN}✓ Extracted EMNIST dataset${RESET}"
                    
                    # Extract any .gz files within the extracted content
                    echo -e "${BLUE}  Extracting compressed files...${RESET}"
                    find . -name "*.gz" -exec gunzip {} \; 2>/dev/null || true
                    
                    # Organize files: move from gzip subdirectory to current directory if needed
                    if [ -d "gzip" ]; then
                        echo -e "${BLUE}  Organizing EMNIST files...${RESET}"
                        mv gzip/* . 2>/dev/null || true
                        rmdir gzip 2>/dev/null || true
                    fi
                    
                    # List extracted EMNIST files
                    echo -e "${BLUE}  Available EMNIST variants:${RESET}"
                    find . -name "*ubyte" | head -10 | while read -r file; do
                        echo -e "    $(basename "$file")"
                    done
                    
                    # Clean up zip file
                    rm -f "$EMNIST_FILE"
                else
                    echo -e "${RED}✗ Failed to extract EMNIST dataset${RESET}"
                    # Clean up corrupted zip
                    rm -f "$EMNIST_FILE"
                fi
            else
                echo -e "${RED}✗ EMNIST zip file appears corrupted${RESET}"
                echo -e "${YELLOW}  Removing corrupted file...${RESET}"
                rm -f "$EMNIST_FILE"
                EMNIST_SUCCESS=false
            fi
        else
            echo -e "${RED}✗ 'unzip' command not found. Please install unzip to extract EMNIST.${RESET}"
            echo -e "${YELLOW}  On Ubuntu/Debian: sudo apt install unzip${RESET}"
            EMNIST_SUCCESS=false
        fi
    else
        echo -e "${RED}✗ No valid EMNIST file found${RESET}"
        EMNIST_SUCCESS=false
    fi
    
    cd "../.."
fi

# ==============================================
# Dataset Information
# ==============================================
echo -e "\n${YELLOW}=== Dataset Summary ===${RESET}"

echo -e "${GREEN}MNIST Dataset (Handwritten Digits):${RESET}"
if [ -f "$MNIST_DIR/train-images-idx3-ubyte" ]; then
    echo -e "  ✓ Training Images: 60,000 samples (28x28 pixels)"
    echo -e "  ✓ Training Labels: 60,000 labels (digits 0-9)"
fi
if [ -f "$MNIST_DIR/t10k-images-idx3-ubyte" ]; then
    echo -e "  ✓ Test Images: 10,000 samples (28x28 pixels)"
    echo -e "  ✓ Test Labels: 10,000 labels (digits 0-9)"
fi

# Check for EMNIST files more robustly
EMNIST_FILE_COUNT=$(find "$EMNIST_DIR" -name "*ubyte" 2>/dev/null | wc -l)
if [ "$EMNIST_FILE_COUNT" -gt 0 ]; then
    echo -e "\n${GREEN}EMNIST Dataset (Extended MNIST - Letters):${RESET}"
    echo -e "  ✓ Multiple variants available in 'data/emnist/' directory"
    echo -e "  ✓ Includes letters (A-Z) and digits (0-9)"
    echo -e "  ✓ Various class configurations (balanced, byclass, letters, etc.)"
    echo -e "  ✓ ${EMNIST_FILE_COUNT} EMNIST files extracted"
    EMNIST_SUCCESS=true
fi

# ==============================================
# Usage Information
# ==============================================
echo -e "\n${YELLOW}=== Usage Information ===${RESET}"
echo -e "${BLUE}Files are ready to use with your MNISTLoader class:${RESET}"
echo -e "  • MNIST files are in the 'data/mnist/' directory"
echo -e "  • Update MNISTLoader to use 'data/mnist/' path"

if [ "$EMNIST_FILE_COUNT" -gt 0 ]; then
    echo -e "\n${BLUE}For EMNIST (you'll need to extend MNISTLoader):${RESET}"
    echo -e "  • EMNIST files are in 'data/emnist/' directory"
    echo -e "  • Consider implementing EMNISTLoader class for letter recognition"
    echo -e "  • Balanced dataset has 47 classes (A-Z, a-z minus duplicates, 0-9)"
else
    echo -e "\n${YELLOW}EMNIST dataset not available:${RESET}"
    echo -e "  • Download manually from: https://www.nist.gov/itl/products-and-services/emnist-dataset"
    echo -e "  • Extract to 'data/emnist/' directory"
    echo -e "  • Run this script again to verify extraction"
fi

echo -e "\n${GREEN}============================================${RESET}"
echo -e "${GREEN}  Dataset preparation completed successfully!${RESET}"
echo -e "${GREEN}============================================${RESET}"

echo -e "\n${BLUE}Ready to build machine learning models!${RESET}"
echo -e "${YELLOW}Update your main.cpp to use 'data/mnist/' path.${RESET}"