# AgriConnect Pakistan

## Simple Agricultural Supply & Cold Chain Management (C++ Console)

![License](https://img.shields.io/badge/license-MIT-green)
![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Language](https://img.shields.io/badge/C%2B%2B-17-orange)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Algorithms](#algorithms)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

---

## 🌾 Overview

AgriConnect Pakistan is a simple, single-file C++ console application. It lets farmers list crops, buyers compare prices and place orders, storage owners allocate cold storage optimally, and transport providers accept delivery requests.

---

## 🔴 Problem Statement

Agriculture forms the backbone of Pakistan's economy, but farmers face challenges:

1. **Limited Access to Cold Storage** - Crops spoil before reaching markets
2. **Lack of Transparency** - Information gaps between producers and buyers
3. **Inefficient Transportation** - Difficulty finding optimal routes
4. **Post-Harvest Losses** - Up to 30-40% of crops wasted

**Solution:** A technology-driven platform using optimized algorithms and data structures.

---

## ✨ Role-Based Features

### 👨‍🌾 Farmer
- Add new crops with type, quantity, quality, price
- View own crops sorted by price (BST traversal)
- Request cold storage for crop preservation
- Request transport services
- Track order deliveries
- View system statistics

### 🛒 Buyer
- View all available crops (sorted by price using BST)
- Compare crop prices (in-order tree traversal)
- Place orders and specify quantities
- Pay for orders
- Request transport services
- Track order and delivery status
- View market analytics and reports

### ❄️ Storage Owner
- View storage requests from farmers (Linked List)
- Allocate storage optimally using Knapsack DP
- Maximize budget utilization
- View available storage centers and capacity
- Monitor utilization rates

### 🚚 Transport Provider
- View transport requests (Linked List)
- Accept requests using Greedy Vehicle Assignment
- Complete deliveries and update orders
- Find optimal routes using Dijkstra's algorithm with Min-Heap
- Track delivery statistics
- View system analytics

---

## 🛠 Quick Start

### Windows (recommended)

```bat
run_simple.bat
```

### Linux/macOS

```bash
chmod +x run_simple.sh
./run_simple.sh
```

### Manual compile

```bash
cd backend_cpp
g++ -std=c++17 -Wall -Wextra -O2 -o agriconnect agriconnect_simple.cpp
./agriconnect
```

---

## 🔢 Algorithms & Data Structures

### Data Structures
- **Hash Table** (User lookup): O(1) average username search with chaining
- **Binary Search Tree** (Crops): O(log n) insertion, automatic price sorting, in-order traversal
- **Linked Lists** (Orders, Transport, Storage, Vehicles, Centers): dynamic growth, no size limits
- **Adjacency List** (City network): space-efficient graph representation
- **Min-Heap** (Dijkstra's priority queue): efficient distance extraction

### Algorithms
- **Bubble Sort**: Compare crop prices (O(n²), simple, educational)
- **Greedy First-Fit**: Assign vehicles to transport requests (O(n))
- **0/1 Knapsack (DP)**: Optimal storage allocation to maximize budget (O(n×W))
- **Dijkstra with Min-Heap**: Find shortest routes between cities (O((V+E)log V))

---

## 📁 Project Structure

```
AgriConnect-Pakistan/
├── backend_cpp/
│   └── agriconnect_simple.cpp      # Optimized C++ backend with all data structures
├── frontend/                       # Static HTML/CSS/JS frontend
│   ├── index.html, login.html, register.html
│   ├── dashboard.html, crops.html, storage.html
│   ├── transport.html, routes.html, reports.html
│   ├── css/style.css
│   └── js/*.js
├── database/
│   └── schema.sql
├── run_simple.bat                  # Windows build/run
├── run_simple.sh                   # Linux/macOS build/run
├── README.md                       # Project overview
└── CONTRIBUTING.md                 # Build instructions
```

---

## 📖 Usage Guide

### Main Menu

When you run the application, you'll see:

```
Main Menu:
1. User Management
2. Crop Management
3. Storage Management
4. Transport Management
5. Order Queue Management
6. Route Planning (Dijkstra)
7. View Statistics
8. View System Information
0. Exit
```

### Test Credentials (Pre-loaded)

The system comes with sample data:

```
Usernames: farmer1, farmer2, buyer1, storage_owner1, transport_provider1, admin
Password: pass123 (or admin123 for admin)
```

### Example Usage Scenarios

#### 1. Find Shortest Route (Dijkstra's Algorithm)

```
Main Menu → 6 (Route Planning) → 1 (Find Shortest Route)
Enter start city: Lahore
Enter destination: Karachi

Output:
Path: Lahore -> Multan -> Karachi
Total Distance: 1322 km
Estimated Time: 16h 32m
Algorithm: Dijkstra O((V+E)logV)
```

#### 2. Allocate Storage (Priority Queue)

```
Main Menu → 3 (Storage Management) → 2 (Allocate Storage)
Enter quantity: 2000

Output:
Storage allocated at: Karachi Agri Hub
Available capacity: 10000 kg
Temperature: 5.0°C
Algorithm: Greedy O(n log n)
```

#### 3. Add New Crop

```
Main Menu → 2 (Crop Management) → 1 (Add New Crop)
Enter crop type: Mango
Enter quantity: 500
Enter quality (A/B/C): A
Enter price per kg: 150
Enter farmer: farmer1

Output: Crop added successfully!
```

#### 4. View Order Queue (FIFO)

```
Main Menu → 5 (Order Queue) → 2 (View All Orders)

Output: Shows all pending orders in FIFO sequence
Position 1: Order ID 1 | User: buyer1 | Crop ID: 2 | Qty: 1000 kg
Position 2: Order ID 2 | User: buyer1 | Crop ID: 3 | Qty: 500 kg
...
```

### Using Data Structures

#### MAP Usage (O(log n) lookup)
```cpp
// Accessing user data
map<string, User> users;
User user = users["farmer1"];  // O(log n)
```

#### SET Usage (O(log n) insertion)
```cpp
// Role categorization
set<string> farmers;
farmers.insert("farmer1");  // O(log n)
```

#### DEQUE Usage (O(1) operations)
```cpp
// Order queue
deque<Order> orders;
orders.push_back(newOrder);  // O(1) enqueue
Order first = orders.front(); // O(1) access
orders.pop_front();          // O(1) dequeue
```

#### PRIORITY QUEUE Usage (O(log n) operations)
```cpp
// Storage allocation
priority_queue<PQElement> pq;
pq.push({score, storageId});  // O(log n)
PQElement best = pq.top();    // O(1)
pq.pop();                     // O(log n)
```

#### GRAPH Usage (Adjacency List)
```cpp
// City network
map<string, vector<Edge>> cityGraph;
cityGraph["Lahore"] = {{{"Islamabad", 375}, {"Multan", 342}}};
```

---

## 📦 Dependencies

- Any C++17-capable compiler (GCC, Clang, MSVC)
- No third-party libraries
- Console-only app; no server required

---

## 🔧 Build Troubleshooting

- Install a compiler:
   - Windows: Visual Studio Build Tools or `mingw-w64`
   - Linux: `sudo apt install build-essential`
   - macOS: `xcode-select --install`
- If `g++` not found, use `cl` (MSVC) from a Developer Command Prompt.

---

## 🎓 Notes

- Code is in one file: [backend_cpp/agriconnect_simple.cpp](backend_cpp/agriconnect_simple.cpp)
- Algorithms are implemented clearly for learning (Bubble Sort, Greedy, Knapsack)

---

## 🤝 Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for build/run instructions and contribution guidelines.

---

## 📄 License

MIT License - Copyright (c) 2026 AgriConnect Pakistan

---

## 📞 Support

For questions or issues:
- Check [CONTRIBUTING.md](CONTRIBUTING.md) for build help
- Review code comments in source files
- Contact: support@agriconnect.pk

---

## 🎯 Key Highlights

✨ **Educational Value** - Clear implementations of data structures and algorithms
✨ **Production Ready** - Professional C++ code with proper structure
✨ **Well Documented** - Extensive comments and complexity analysis
✨ **Cross-Platform** - Works on Windows, Linux, and macOS
✨ **No Dependencies** - Pure STL implementation
✨ **Interactive** - Console-based menu system
✨ **Extensible** - Easy to add new features

---

**Version:** 1.0.0
**Last Updated:** January 2026
**Language:** C++17
**Status:** Ready to Use ✓