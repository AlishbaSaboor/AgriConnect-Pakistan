# How to Run AgriConnect Pakistan

## Quick Start Guide

### Prerequisites
- Windows (PowerShell or Command Prompt)
- C++17 compiler (MSVC or MinGW/GCC)
- Python 3 (to serve the frontend)

---

## Running the Backend (C++)

### Option 1: PowerShell
```powershell
cd backend_cpp
g++ -std=c++17 -O2 -o integrated_server.exe integrated_server.cpp -lws2_32 -static-libgcc -static-libstdc++; .\integrated_server.exe
```

### Option 1b: Command Prompt (CMD)
```batch
cd backend_cpp
g++ -std=c++17 -O2 -o integrated_server.exe integrated_server.cpp -lws2_32 -static-libgcc -static-libstdc++ && .\integrated_server.exe
```

Notes:
- The backend uses Winsock2 (provided by Windows SDK/Visual Studio).
- If port 8080 is busy, stop existing processes using that port or change the port in `integrated_server.cpp`.

---

## Running the Frontend (HTML/JavaScript)

The frontend is static HTML/CSS/JS and calls the backend at `http://localhost:8080` (configured in `frontend/js/api-config.js`).

### Option 1: Direct File Opening
```powershell
# Simply double-click:
frontend/index.html
```

### Option 2: Using Python HTTP Server
```powershell
cd frontend
python -m http.server 8000
# Then visit: http://localhost:8000
```

### Option 3: Using Node.js http-server
```powershell
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

### "Connection error. Make sure C++ server is running on port 8080."
- Ensure backend is listening: `netstat -ano | findstr :8080`
- Test endpoint directly: open http://localhost:8080/status in a browser
- Confirm `frontend/js/api-config.js` has `API_BASE_URL = 'http://localhost:8080'`
- Hard refresh the browser (Ctrl+F5) or clear cache
- Temporarily allow the app through Windows Firewall if blocked

### "Compiler not found"
Install one of:
- Visual Studio Community (MSVC) or Build Tools
- MinGW-w64 (GCC for Windows)

### Backend compiles but doesn't start
- Run `server.exe` from inside the `backend_cpp` folder
- Check port 8080 usage: `netstat -ano | findstr :8080`
- With MinGW, try: `-static-libgcc -static-libstdc++`

---

## Using the Application

Key backend endpoints (mock server):
- `GET /status` → server health
- `POST /login` → success for demo payload; invalid body returns `{ success: false, message: "Invalid credentials. Please register first!" }`
- `POST /register` → demo registration success
- `GET /users`, `GET /crops` → sample data

---

## Contributing to the Project

### Adding / Updating Backend Endpoints
1. Open `backend_cpp/integrated_server.cpp`
2. Add your handler logic in `handleRequest()` or extend routing in `handleClient()`
3. Recompile:
   ```powershell
   cd backend_cpp
   # PowerShell: use semicolon (;)
   g++ -std=c++17 -O2 -o integrated_server.exe integrated_server.cpp -lws2_32 -static-libgcc -static-libstdc++; .\integrated_server.exe
   ```
   
   Or in Command Prompt (CMD): use && operator
   ```batch
   g++ -std=c++17 -O2 -o integrated_server.exe integrated_server.cpp -lws2_32 -static-libgcc -static-libstdc++ && .\integrated_server.exe
   ```

### Code Style
- Use clear comments for data structures
- Document algorithm complexity when relevant
- Follow existing naming conventions
- Add inline documentation

---

## Database Setup (Optional)

If you want to use MySQL:
```powershell
# 1. Install MySQL Server
# 2. Import schema
mysql -u root -p < database/schema.sql

# 3. Update connection in code (future enhancement)
```

---