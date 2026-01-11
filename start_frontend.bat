@echo off
cd /d "%~dp0frontend"
echo Starting AgriConnect Frontend Server...
python -m http.server 8000
pause
