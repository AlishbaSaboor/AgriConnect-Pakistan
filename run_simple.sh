#!/bin/bash

echo ""
echo "======================================"
echo "  AgriConnect Pakistan - Simple Build"
echo "======================================"
echo ""

cd backend_cpp || { echo "Failed to cd backend_cpp"; exit 1; }

if command -v g++ >/dev/null 2>&1; then
    echo "Detected g++. Compiling..."
    g++ -std=c++17 -Wall -Wextra -O2 -o agriconnect_simple agriconnect_simple.cpp
    status=$?
else
    echo "g++ not found. Please install build-essential (Linux) or Xcode CLT (macOS)."
    exit 1
fi

if [ $status -eq 0 ]; then
    echo ""
    echo "Compilation successful!"
    echo ""
    echo "Running AgriConnect Pakistan..."
    echo ""
    ./agriconnect_simple
else
    echo ""
    echo "Compilation failed! Please check for errors."
    echo ""
    exit $status
fi
