@echo off
setlocal
echo ======================================
echo   AgriConnect Pakistan - Simple Build
echo ======================================

cd backend_cpp || goto fail_cd

:: Detect g++ (MinGW)
where g++ >nul 2>&1
if not errorlevel 1 goto use_gpp

:: Detect MSVC cl
where cl >nul 2>&1
if not errorlevel 1 goto use_cl

echo No C++ compiler found.
echo Install MinGW (g++) or Visual Studio Build Tools (cl).
exit /b 1

:use_gpp
echo Detected g++ (MinGW). Compiling...
g++ -std=c++17 -Wall -Wextra -O2 -o agriconnect_simple.exe agriconnect_simple.cpp
if errorlevel 1 goto build_failed
goto run

:use_cl
echo Detected MSVC cl. Compiling...
cl /std:c++17 /EHsc agriconnect_simple.cpp /Fe:agriconnect_simple.exe
if errorlevel 1 goto build_failed
goto run

:run
if exist agriconnect_simple.exe (
  echo Running AgriConnect Pakistan...
  agriconnect_simple.exe
  goto end
)
echo Build output not found: agriconnect_simple.exe
exit /b 1

:build_failed
echo Compilation failed.
exit /b 1

:fail_cd
echo Failed to change directory to backend_cpp.
exit /b 1

:end
endlocal
