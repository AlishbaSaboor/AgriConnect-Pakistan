# How to Run AgriConnect Pakistan

## Quick Start Guide

### Prerequisites
- C++ compiler (MSVC, GCC, or Clang)
- CMake 3.10+ (optional, for easier builds)
- Terminal/Command Prompt

---

## Running the Backend (C++)

### Option 1: Using CMake & Visual Studio

```bash
# 1. Open Command Prompt or PowerShell
cd AgriConnect-Pakistan

# 2. Create build directory
mkdir build
cd build

# 3. Configure with CMake (choose your Visual Studio version)
cmake .. -G "Visual Studio 16 2019"
# OR
cmake .. -G "Visual Studio 17 2022"

# 4. Build the project
cmake --build . --config Release

# 5. Run the executable
Release\AgriConnect_Backend.exe
```

### Option 2: Using VS Code

If you prefer using VS Code instead of Visual Studio:

```bash
# 1. Open project folder in VS Code
# 2. Install "C/C++" extension by Microsoft
# 3. Open terminal in VS Code (Ctrl + `)
# 4. Navigate to backend_cpp folder
cd backend_cpp

# 5. Compile directly with g++ (if you have MinGW or similar)
g++ -std=c++17 -o AgriConnect_Backend.exe main.cpp agriconnect.cpp

# 6. Run
.\AgriConnect_Backend.exe
```

### Option 3: Direct Compilation (Windows - MSVC)

```bash
# Open Developer Command Prompt for Visual Studio
cd backend_cpp
cl /std:c++17 /EHsc main.cpp agriconnect.cpp /Fe:AgriConnect_Backend.exe
AgriConnect_Backend.exe
```

### Option 4: Using Makefile (if available)

```bash
# From project root
make
```

---

## Running the Frontend (HTML/JavaScript)

The frontend is standalone HTML/CSS/JavaScript and doesn't need a backend connection.

### Option 1: Direct File Opening
```bash
# Simply double-click on:
frontend/index.html
```

### Option 2: Using Python HTTP Server
```bash
cd frontend
python -m http.server 8000
# Then visit: http://localhost:8000
```

### Option 3: Using Node.js http-server
```bash
cd frontend
npx http-server -p 8000
# Then visit: http://localhost:8000
```

### Option 4: Using VS Code Live Server
1. Open VS Code
2. Install "Live Server" extension
3. Right-click on `frontend/index.html`
4. Select "Open with Live Server"

---

## Troubleshooting

### "CMake not found"
- Download from: https://cmake.org/download/
- Add to PATH during installation

### "Compiler not found"
**Windows:**
- Install Visual Studio Community (free): https://visualstudio.microsoft.com/
- Or install Build Tools for Visual Studio

### "Visual Studio version mismatch"
If you get an error about Visual Studio version, specify your installed version:
```bash
cmake .. -G "Visual Studio 16 2019"  # For VS 2019
cmake .. -G "Visual Studio 17 2022"  # For VS 2022
```

Or use MinGW:
```bash
cmake .. -G "MinGW Makefiles"
```

### Build succeeds but exe doesn't run
Check if the executable exists:
```bash
dir Release\AgriConnect_Backend.exe  # Windows
```

Run from the correct directory (build folder).

---

## Using the Application

Once the backend runs, you'll see a menu:

```
Main Menu:
1. Register
2. Login
3. View All Users
4. View Statistics
5. Find Shortest Route
0. Exit
```

### Test Credentials (Pre-loaded)
- **Username:** farmer1, buyer1, storage_owner(ColdHub, AgriStore, FarmSafe, FreshKeep), transport_provider (FastMove, AgriTrans, GreenWay)
- **Password:** pass123 (for all users)

---

## Contributing to the Project

### Adding New Features

1. **Implement the functions:**
   - Open `backend_cpp/agriconnect_simple.cpp`
   - Write your implementation

2. **Rebuild:**
   ```bash
   cd build
   cmake --build . --config Release
   ```

### Code Style
- Use clear comments for data structures
- Document algorithm complexity
- Follow existing naming conventions
- Add inline documentation

---

## Database Setup (Optional)

If you want to use MySQL:

```bash
# 1. Install MySQL Server
# 2. Import schema
mysql -u root -p < database/schema.sql

# 3. Update connection in code (future enhancement)
```

---
