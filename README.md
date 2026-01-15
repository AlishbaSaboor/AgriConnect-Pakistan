# AgriConnect Pakistan

## Agricultural Supply & Cold Chain (C++ Backend + Static Frontend)

![License](https://img.shields.io/badge/license-MIT-green)
![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Language](https://img.shields.io/badge/C%2B%2B-17-orange)
![Platform](https://img.shields.io/badge/Platform-Windows-blue)

---

## 🌾 Overview

AgriConnect Pakistan provides a lightweight C++ HTTP backend and a static HTML/CSS/JS frontend. It lets farmers list crops, buyers compare prices and place orders, storage owners allocate cold storage, and transport providers accept delivery requests.

---

## 🔴 Problem Statement

Agriculture forms the backbone of Pakistan's economy, but farmers face challenges:

1. **Limited Access to Cold Storage** - Crops spoil before reaching markets
2. **Lack of Transparency** - Information gaps between producers and buyers
3. **Inefficient Transportation** - Difficulty finding optimal routes
4. **Post - Harvest Losses** - Up to 30 - 40 % of crops wasted

**Solution:** A technology - driven platform using optimized algorithms and data structures.

---

## ✨ Role - Based Features

### 👨‍🌾 Farmer
- Add new crops with type, quantity, quality, price
- View own crops sorted by price(BST traversal)
- Request cold storage for crop preservation
- Request transport services
- Track order deliveries
- View system statistics

### 🛒 Buyer
- View all available crops(sorted by price using BST)
- Compare crop prices(in - order tree traversal)
- Place orders and specify quantities
- Pay for orders
- Request transport services
- Track order and delivery status
- View market analytics and reports

### ❄️ Storage Owner
- View storage requests from farmers(Linked List)
- Allocate storage optimally using Knapsack DP
- Maximize budget utilization
- View available storage centers and capacity
- Monitor utilization rates

### 🚚 Transport Provider
- View transport requests(Linked List)
- Accept requests using Greedy Vehicle Assignment
- Complete deliveries and update orders
- Find optimal routes using Dijkstra's algorithm with Min-Heap
- Track delivery statistics
- View system analytics

---
## 🛠 Quick Start

### Windows

#### Option 1: Console Application with JSON Persistence 

```bash
cd backend_cpp
compile.bat              # Auto-compiles and runs
```

**Or manually compile and run:**

```powershell
# PowerShell (Windows)
cd backend_cpp
g++ -std=c++17 -O2 -o agriconnect.exe agriconnect_simple.cpp; .\agriconnect.exe
```

```batch
@REM CMD/Batch (Windows)
cd backend_cpp
g++ -std=c++17 -O2 -o agriconnect.exe agriconnect_simple.cpp && .\agriconnect.exe
```

#### Option 2: HTTP Backend + Frontend (Full Web Application)

```bash
start_backend.bat   # Starts C++ backend on http://localhost:8080
start_frontend.bat  # Starts static frontend on http://localhost:8000
```

#### Option 3: Manual Setup

```bash
# Backend (in a new terminal)
cd backend_cpp
.\integrated_server.exe

# Frontend (in a second terminal)
cd ../frontend
python -m http.server 8000
```

#### Option 4: Compile Backend Yourself (MinGW or MSVC)

```bash
cd backend_cpp
# For console app with JSON persistence:
g++ -std=c++17 -O2 -o agriconnect.exe agriconnect_simple.cpp

# For HTTP server:
g++ -std=c++17 -O2 -o integrated_server.exe integrated_server.cpp -lws2_32 -static-libgcc -static-libstdc++
```

---

## �🔢 Algorithms & Data Structures

### Data Structures
- **Hash Table** (User lookup) : O(1) average username search with chaining
- **Binary Search Tree** (Crops) : O(log n) insertion, automatic price sorting, in - order traversal
- **Linked Lists** (Orders, Transport, Storage, Vehicles, Centers) : dynamic growth, no size limits
- **Adjacency List** (City network) : space - efficient graph representation
- **Min - Heap** (Dijkstra's priority queue): efficient distance extraction

### Algorithms
- **Greedy First - Fit**: Assign vehicles to transport requests(O(n))
- **0 / 1 Knapsack(DP)**: Optimal storage allocation to maximize budget(O(n×W))
- **Dijkstra with Min - Heap**: Find shortest routes between cities(O((V + E)log V))

---

## 📁 Project Structure

```
AgriConnect-Pakistan/
├── backend_cpp/
│   ├── agriconnect_simple.cpp        # Main console application with all data structures
│   ├── json_handler.h                # JSON persistence header
│   ├── json_handler.cpp              # JSON save/load implementation
│   ├── compile.bat                   # One-click compilation script
│   ├── integrated_server.cpp         # HTTP backend (Winsock) + real data structures
│   ├── integrated_server.exe         # Compiled HTTP server
│   ├── agriconnect.exe               # Compiled console app with JSON persistence
│   └── data/                         # JSON data storage (auto-created)
│       ├── users.json                # Persistent user data
│       ├── crops.json                # Persistent crop listings
│       ├── orders.json               # Persistent orders
│       ├── storage_requests.json     # Persistent storage requests
│       ├── transport_requests.json   # Persistent transport requests
│       ├── vehicles.json             # Persistent vehicle data
│       └── storage_centers.json      # Persistent storage centers
├── frontend/                         # Static HTML/CSS/JS frontend
│   ├── index.html, login.html, register.html
│   ├── dashboard.html, crops.html, storage.html
│   ├── transport.html, routes.html, reports.html
│   ├── css/style.css
│   └── js/*.js (api-config, login, register, dashboard, etc.)
├── database/
│   └── schema.sql                    # MySQL schema (optional migration)
├── start_backend.bat                 # Start HTTP backend (Windows)
├── start_frontend.bat                # Start frontend (Windows)
├── README.md                         # Project overview
└── CONTRIBUTING.md                   # Build instructions
```

---

## 📖 Usage Guide

### Main Menu

When you run the application, you'll see:

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

The system comes with sample data:

```
Usernames: farmer1, farmer2, buyer1, storage_owner(ColdHub, AgriStore, FarmSafe, FreshKeep), transport_provider (FastMove, AgriTrans, GreenWay)
Password: pass123 (for all users)
```

### Example Usage Scenarios

#### 1. Find Shortest Route

```
Main Menu → 5 (Find Shortest Route)
Enter start city: Lahore
Enter destination: Karachi

Output:
Path: Lahore -> Multan -> Karachi
Total Distance: 1322 km
Estimated Time: 16h 32m
```

#### 2. Allocate Storage (Priority Queue)

```
Main Menu → Login as storage owner → Storage menu → Allocate Storage
Enter quantity: 2000

Output:
Storage allocated at: Karachi Agri Hub
Available capacity: 10000 kg
Temperature: 5.0°C
```

#### 3. Add New Crop

```
Main Menu → Login as farmer → Crop menu → Add New Crop
Enter crop type: Mango
Enter quantity: 500
Enter quality (A/B/C): A
Enter price per kg: 150
Enter farmer: farmer1

Output: Crop added successfully!
```

---

## 📦 Dependencies

- Windows with a C++17-capable compiler (MSVC or MinGW)
- Winsock2 (provided by Windows SDK/Visual Studio)
- Python 3 (for serving the static frontend on port 8000)
- No third-party server libraries

Backend runs on http://localhost:8080 and the frontend on http://localhost:8000. The frontend uses `frontend/js/api-config.js` with `API_BASE_URL = 'http://localhost:8080'`.

---

## 🤝 Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md) for build/run instructions and contribution guidelines.

---

## 📄 License

MIT License - Copyright (c) 2026 AgriConnect Pakistan

---