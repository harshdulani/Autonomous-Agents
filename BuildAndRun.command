#!/bin/bash

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed or not in PATH."
    exec $SHELL
    exit 1
fi
echo "Found CMake."

# Ensure script runs from its own directory
cd "$(dirname "$0")" || exit 1

# Check if "build" exists and remove it
if [ -e "build" ]; then
    rm -rf build
    echo "Removed existing 'build'"
fi

# Create and enter the build directory
mkdir build
cd build || exit 1

echo "Build directory created and entered."
echo ""
echo ""
echo "Start: Configure the project and generate build files:"
echo ""
echo ""
cmake -G Xcode ..
echo ""
echo ""
echo "Start: Build the project using the generated build files:"
echo ""
echo ""
cmake --build . --config Release
echo ""
echo ""
echo "Executable should be generated at \build\Source\Core\Driver\Release\Asteroids"
"./Source/Core/Driver/Release/Asteroids"