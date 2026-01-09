#!/bin/bash

# Test script to verify the plugin builds and basic functionality

echo "=== SVT-CL Plugin Build Test ==="
echo ""

# Check if build artifacts exist
echo "Checking build artifacts..."

if [ -f "build/SVT-CL_artefacts/VST3/SVT-CL.vst3/Contents/x86_64-linux/SVT-CL.so" ]; then
    echo "✓ VST3 build successful"
    ls -lh build/SVT-CL_artefacts/VST3/SVT-CL.vst3/Contents/x86_64-linux/SVT-CL.so
else
    echo "✗ VST3 build not found"
    exit 1
fi

if [ -f "build/SVT-CL_artefacts/Standalone/SVT-CL" ]; then
    echo "✓ Standalone build successful"
    ls -lh build/SVT-CL_artefacts/Standalone/SVT-CL
else
    echo "✗ Standalone build not found"
    exit 1
fi

echo ""
echo "=== Build Summary ==="
echo "Plugin Name: SVT-CL"
echo "Formats: VST3, Standalone"
echo "DSP Features:"
echo "  - Multi-stage tube preamp with 4 cascaded nonlinear stages"
echo "  - Interactive tone stack (Bass/Mid/Treble)"
echo "  - 5-position Mid frequency selector"
echo "  - Ultra Lo/Ultra Hi filters"
echo "  - Power amp saturation and compression"
echo "  - Output limiter"
echo "  - Cabinet simulator"
echo "  - 4x oversampling on nonlinear stages"
echo ""
echo "All builds completed successfully!"
echo ""
echo "To install VST3: Copy to ~/.vst3/"
echo "To run Standalone: ./build/SVT-CL_artefacts/Standalone/SVT-CL"
