#!/bin/bash
# SuperBassman Build Script
# Builds the VST3 and Standalone versions of the plugin

set -e

echo "==================================="
echo "Building SuperBassman Plugin"
echo "==================================="

# Check if build directory exists
if [ -d "build" ]; then
    echo "Cleaning previous build..."
    rm -rf build
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
make -j$(nproc)

echo "==================================="
echo "Build Complete!"
echo "==================================="
echo "VST3 Plugin: build/SuperBassman_artefacts/VST3/SuperBassman.vst3"
echo "Standalone: build/SuperBassman_artefacts/Standalone/SuperBassman"
echo "==================================="
