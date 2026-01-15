@echo off
REM AgriConnect Pakistan - Build Script for Windows
REM This script compiles the C++ backend with JSON persistence

echo.
echo ================================
echo AgriConnect Pakistan - Compiler
echo ================================
echo.

REM Check if g++ is installed
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ compiler not found!
    echo Please install MinGW or Visual Studio C++ compiler
    pause
    exit /b 1
)

echo [1/3] Compiling JSON handler...
g++ -std=c++17 -c json_handler.cpp -o json_handler.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile json_handler.cpp
    pause
    exit /b 1
)

echo [2/3] Linking with main program...
g++ -std=c++17 -O2 -o agriconnect.exe agriconnect_simple.cpp json_handler.o
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to link program
    pause
    exit /b 1
)

echo [3/3] Build successful!
echo.
echo ================================
echo Build Complete!
echo ================================
echo.
echo Running: agriconnect.exe
echo.

.\agriconnect.exe
pause
