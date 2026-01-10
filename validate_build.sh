#!/bin/bash
# Ampeg SVT Bass Amp Plugin - Build Validation Script
# This script validates that the plugin builds correctly

set -e  # Exit on error

echo "========================================="
echo "Ampeg SVT Bass Amp Plugin - Build Test"
echo "========================================="
echo ""

# Check prerequisites
echo "Checking prerequisites..."

# Check CMake
if ! command -v cmake &> /dev/null; then
    echo "❌ ERROR: CMake is not installed"
    echo "   Install with: sudo apt-get install cmake"
    exit 1
fi
echo "✅ CMake found: $(cmake --version | head -1)"

# Check C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "❌ ERROR: No C++ compiler found"
    echo "   Install with: sudo apt-get install build-essential"
    exit 1
fi

if command -v g++ &> /dev/null; then
    echo "✅ GCC found: $(g++ --version | head -1)"
fi

# Check if JUCE submodule exists
if [ ! -d "JUCE/modules" ]; then
    echo "⚠️  JUCE submodule not initialized"
    echo "   Initializing submodule..."
    git submodule update --init --recursive
    if [ $? -ne 0 ]; then
        echo "❌ ERROR: Failed to initialize JUCE submodule"
        exit 1
    fi
    echo "✅ JUCE submodule initialized"
else
    echo "✅ JUCE submodule found"
fi

# Check Linux dependencies (if on Linux)
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo ""
    echo "Checking Linux dependencies..."
    
    DEPS=(
        "libasound2-dev"
        "libfreetype6-dev"
        "libx11-dev"
        "libxext-dev"
        "libxrandr-dev"
        "libxinerama-dev"
        "libxcursor-dev"
        "libgl1-mesa-dev"
    )
    
    MISSING_DEPS=()
    for dep in "${DEPS[@]}"; do
        if ! dpkg -l | grep -q "^ii  $dep"; then
            MISSING_DEPS+=("$dep")
        fi
    done
    
    if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
        echo "⚠️  Missing dependencies: ${MISSING_DEPS[*]}"
        echo "   Install with:"
        echo "   sudo apt-get install ${MISSING_DEPS[*]}"
        read -p "   Install now? (y/n) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            sudo apt-get update
            sudo apt-get install -y "${MISSING_DEPS[@]}"
        else
            echo "❌ Cannot continue without dependencies"
            exit 1
        fi
    else
        echo "✅ All Linux dependencies installed"
    fi
fi

# Create build directory
echo ""
echo "Setting up build directory..."
if [ -d "build" ]; then
    echo "⚠️  Build directory exists, cleaning..."
    rm -rf build
fi
mkdir build
cd build
echo "✅ Build directory created"

# Configure with CMake
echo ""
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo "❌ ERROR: CMake configuration failed"
    exit 1
fi
echo "✅ CMake configuration successful"

# Build
echo ""
echo "Building plugin (this may take a few minutes)..."
cmake --build . --config Release
if [ $? -ne 0 ]; then
    echo "❌ ERROR: Build failed"
    exit 1
fi
echo "✅ Build successful!"

# Verify outputs
echo ""
echo "Verifying build outputs..."

if [ -d "AmpegSVT_artefacts/VST3" ]; then
    echo "✅ VST3 plugin built successfully"
    ls -lh "AmpegSVT_artefacts/VST3/"
else
    echo "❌ ERROR: VST3 plugin not found"
    exit 1
fi

if [ -d "AmpegSVT_artefacts/Standalone" ]; then
    echo "✅ Standalone application built successfully"
    ls -lh "AmpegSVT_artefacts/Standalone/"
else
    echo "❌ ERROR: Standalone application not found"
    exit 1
fi

# Summary
echo ""
echo "========================================="
echo "✅ BUILD VALIDATION SUCCESSFUL!"
echo "========================================="
echo ""
echo "Build artifacts:"
echo "  VST3:       build/AmpegSVT_artefacts/VST3/"
echo "  Standalone: build/AmpegSVT_artefacts/Standalone/"
echo ""
echo "To install the VST3 plugin:"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "  cp -r 'AmpegSVT_artefacts/VST3/Ampeg SVT Bass Amp.vst3' ~/.vst3/"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "  cp -r 'AmpegSVT_artefacts/VST3/Ampeg SVT Bass Amp.vst3' ~/Library/Audio/Plug-Ins/VST3/"
fi
echo ""
echo "See README.md for full documentation."
echo ""
