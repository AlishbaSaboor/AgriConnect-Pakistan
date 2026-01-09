# AgriConnect Pakistan

## Smart Agricultural Supply & Cold Chain Management System

![License](https://img.shields.io/badge/license-MIT-green)
![Version](https://img.shields.io/badge/version-2.0.0-blue)
![Python](https://img.shields.io/badge/Python-Flask-blue)
![Frontend](https://img.shields.io/badge/Frontend-HTML%2FJS%2FCSS-orange)

---

## 📋 Table of Contents
- [Overview](#overview)
- [Problem Statement](#problem-statement)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [Data Structures & Algorithms](#data-structures--algorithms)
- [Quick Start](#quick-start)
- [Installation & Setup](#installation--setup)
- [Project Structure](#project-structure)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [License](#license)

---

## 🌾 Overview

AgriConnect Pakistan is a digital platform designed to optimize agricultural supply and cold chain management. The system connects farmers, buyers, cold storage centers, and transport providers, reducing post-harvest losses and improving market access.

### Key Objectives
- Connect all stakeholders in the agricultural supply chain
- Reduce post-harvest losses through efficient cold storage allocation
- Optimize transport routing using advanced algorithms (Dijkstra's)
- Provide transparent pricing and payment systems
- Enable real-time tracking and monitoring

---

## 🔴 Problem Statement

Agriculture forms the backbone of Pakistan's economy, but small-scale farmers face significant challenges:

1. **Limited Access to Cold Storage** - Perishable crops spoil before reaching buyers
2. **Lack of Transparency** - Middlemen exploit both farmers and buyers
3. **Inefficient Transportation** - Difficulty finding reliable and cost-effective logistics
4. **Post-Harvest Losses** - Up to 30-40% of crops waste due to poor infrastructure

**Solution:** A centralized, technology-driven platform connecting all stakeholders.

---

## ✨ Features

### 🔐 User Management
- Secure registration and login for all user types
- Role-based access control (Farmer, Buyer, Storage Owner, Transport Provider, Admin)
- User verification system

### 🌾 Crop Management
- Farmers can list crops with type, quantity, quality grade, and price
- Buyers can browse and search available crops
- Quality grading system (A, B, C)
- Real-time crop availability updates

### ❄️ Cold Storage Management
- View storage centers by location, capacity, and temperature
- **Priority Queue Algorithm** for optimal storage allocation
- Real-time capacity monitoring
- Temperature tracking to maintain crop quality

### 🚚 Transport Management
- Schedule and book transport services
- Live tracking of deliveries
- **Dijkstra's Algorithm** for shortest route calculation
- Vehicle capacity and route optimization

### 💳 Payment System
- Cash payment option
- Digital payments (EasyPaisa, JazzCash, Bank Transfer)
- Secure transaction processing
- Payment history and receipts

### 📊 Reports & Analytics
- User statistics by role
- Crop listing summaries
- Storage utilization reports
- Transport service analytics
- Algorithm performance metrics

---

## 🛠️ Technology Stack

### Frontend
- **HTML5** - Structure
- **CSS3** - Styling with responsive design
- **Vanilla JavaScript** - Interactive functionality
- **REST API** - Communication with backend

### Backend
- **Python 3** - Modern, beginner-friendly language
- **Flask** - Lightweight web framework
- **In-Memory Data Structures** - No database required
  - `dict` - HashMap for users, crops, storage, transport
  - `set` - For filtering and unique values
  - `heapq` - Priority queue for storage allocation
  - `collections.deque` - Queue for order processing
  - `dict` - Graph structure for routes

### Algorithms
- **Dijkstra's Algorithm** - Shortest path routing (O((V+E) log V))
- **Priority Queue (Heap)** - Storage allocation (O(log n))
- **HashMap** - O(1) lookups for all entities
- **BFS/Graph Traversal** - Route optimization

---

## 📐 Data Structures & Algorithms

### Data Structures Used

1. **HashMap (Python dict)** - Fast lookups
   - Time Complexity: O(1) average
   - Usage: Users, crops, storage, transport data
   - Example: `users_db = {}`, `crops_db = {}`

2. **HashSet (Python set)** - Unique collections
   - Time Complexity: O(1) lookup
   - Usage: Role filtering, unique IDs
   - Example: `farmer_ids = set()`

3. **Priority Queue (heapq)** - Storage allocation
   - Time Complexity: O(log n) insertion
   - Usage: Finding optimal storage based on score
   - Example: `heapq.heappush(pq, (-score, storage))`

4. **Queue (collections.deque)** - Order processing
   - Time Complexity: O(1) enqueue/dequeue
   - Usage: FIFO order management
   - Example: `orders_queue = deque()`

5. **Graph (Adjacency List)** - City network
   - Time Complexity: O(V + E) traversal
   - Usage: Route finding with Dijkstra's
   - Example: `graph = {city: {neighbor: distance}}`

### Algorithms Implemented

#### 1. Dijkstra's Shortest Path Algorithm
```
Purpose: Find shortest route between cities
Time Complexity: O((V + E) log V)
Space Complexity: O(V)

Implementation: backend/app.py (calculate_shortest_path)
Cities: Lahore, Karachi, Islamabad, Multan, Faisalabad, 
        Peshawar, Quetta, Sialkot
```

#### 2. Priority Queue Storage Allocation
```
Purpose: Optimal cold storage selection
Time Complexity: O(n log n) - heap operations
Space Complexity: O(n)

Scoring Formula: 
score = available_capacity - (|temperature - 4°C| × 10)

Implementation: backend/app.py (allocate_optimal_storage)
```

#### 3. HashMap Operations
```
Purpose: Fast CRUD operations for all entities
Time Complexity: O(1) average
Space Complexity: O(n)

Implementation: All API endpoints use dict for storage
```

---

## 📦 Installation & Setup

### Prerequisites

1. **Python 3.8 or higher**
   - Download: https://www.python.org/downloads/
   - Make sure to check "Add Python to PATH" during installation

2. **Web Browser**
   - Chrome, Firefox, Edge, or Safari (latest version)

### Quick Start (3 Steps)

```bash
# Step 1: Clone or download the project
cd AgriConnect-Pakistan

# Step 2: Install dependencies (one-time setup)
pip install -r backend/requirements.txt

# Step 3: Run the application
python run.py
```

**That's it!** The browser will automatically open to http://localhost:8000

- Backend API runs on: http://localhost:5000
- Frontend UI runs on: http://localhost:8000
- Press `Ctrl+C` to stop both servers

### Manual Setup (Alternative)

If you want more control:

**Terminal 1 - Backend:**
```bash
python backend/app.py
```

**Terminal 2 - Frontend:**
```bash
cd frontend
python -m http.server 8000
```

Then open http://localhost:8000 in your browser.

---

## 🚀 How to Run

### Step 1: Start Backend Server

```bash
# In the project root directory
python backend/app.py

# You should see:
# * Running on http://127.0.0.1:5000
```

### Step 2: Open Frontend

**Option A: Direct File Access**
1. Navigate to the `frontend` folder
2. Double-click `index.html` to open in browser

**Option B: Using Python HTTP Server (Recommended)**
```bash
# In a new terminal, navigate to frontend folder
cd frontend

# Start simple HTTP server
python -m http.server 8000

# Open browser to http://localhost:8000
```

**Option C: Using VS Code Live Server**
1. Install "Live Server" extension in VS Code
2. Right-click on `frontend/index.html`
3. Select "Open with Live Server"

### Step 3: Use the Application

**Default Admin Credentials:**
- Username: `admin`
- Password: `admin123`

**Sample Data:**
- Pre-loaded with sample users, crops, storage, and transport data
- You can also register new users

### API Endpoints

Backend runs on `http://localhost:5000/api`

**Available Endpoints:**
- `POST /api/register` - Register new user
- `POST /api/login` - User login
- `GET /api/crops` - Get all crops
- `POST /api/crops` - Add new crop
- `GET /api/storage` - Get all storage centers
- `POST /api/storage/allocate` - Allocate optimal storage (Priority Queue)
- `GET /api/transport` - Get transport services
- `POST /api/routes/shortest` - Calculate shortest path (Dijkstra's)
- `GET /api/orders` - Get orders queue
- `POST /api/orders` - Add order to queue
- `GET /api/stats` - Get system statistics
- `GET /api/info` - Get algorithm information

---

## 📁 Project Structure

```
AgriConnect-Pakistan/
│
├── backend/
│   ├── app.py                  # Flask REST API server
│   └── requirements.txt        # Python dependencies
│
├── frontend/
│   ├── index.html              # Home page
│   ├── register.html           # User registration
│   ├── login.html              # User login
│   ├── dashboard.html          # User dashboard
│   ├── crops.html              # Crop management
│   ├── storage.html            # Storage management
│   ├── transport.html          # Transport management
│   ├── routes.html             # Route finder (Dijkstra's)
│   ├── reports.html            # Analytics & reports
│   │
│   ├── css/
│   │   └── style.css           # All styling
│   │
│   └── js/
│       ├── api-config.js       # API configuration
│       ├── main.js             # Home page logic
│       ├── register.js         # Registration logic
│       ├── login.js            # Login logic
│       ├── dashboard.js        # Dashboard logic
│       ├── crops.js            # Crop management
│       ├── storage.js          # Storage management
│       ├── transport.js        # Transport management
│       ├── routes.js           # Route finder UI
│       └── reports.js          # Reports generation
│
├── USER_GUIDE.md               # Complete user documentation
└── README.md                   # This file
```

---

## 📖 Usage Guide

### For Farmers

1. **Register** as a Farmer
2. **Login** to your account
3. Navigate to **Crop Management**
4. Click **Add New Crop**
5. Fill in crop details (type, quantity, quality, price)
6. Submit to list your crop
7. View all your listed crops

### For Buyers

1. **Register** as a Buyer
2. **Login** to your account
3. Navigate to **Crop Management**
4. Browse available crops
5. Search by crop type
6. View crop details and farmer information

### For Storage Owners

1. **Register** as a Storage Owner
2. **Login** to your account
3. Navigate to **Cold Storage Management**
4. Click **Add Storage Center**
5. Fill in storage details (name, location, capacity, temperature)
6. Submit to register your storage facility
7. Monitor storage utilization

### For Transport Providers

1. **Register** as a Transport Provider
2. **Login** to your account
3. Navigate to **Transport Management**
4. Click **Add Transport Service**
5. Fill in transport details (type, capacity, route, pricing)
6. Submit to register your service
7. Track deliveries

### Using Algorithms

#### Storage Allocation (Priority Queue with Heap)
1. Go to **Cold Storage Management**
2. Scroll to **Optimal Storage Allocation**
3. Enter quantity to store (kg)
4. Click **Find Best Storage**
5. Backend uses `heapq` to find optimal storage based on:
   - Available capacity
   - Temperature optimization (closer to 4°C is better)
   - Algorithm score (higher is better)

#### Route Finding (Dijkstra's Algorithm)
1. Go to **Route Finder**
2. Select **Start City**
3. Select **Destination City**
4. Click **Calculate Shortest Route**
5. Backend calculates shortest path using Dijkstra's algorithm
6. View results:
   - Shortest distance (km)
   - Estimated time
   - Complete path with all intermediate cities

**For complete documentation, see [USER_GUIDE.md](USER_GUIDE.md)**

---

##  Dependencies

### Backend (Python)

```txt
flask>=2.3.0
```

Install with:
```bash
pip install -r backend/requirements.txt
```

**Built-in Python Libraries Used:**
- `heapq` - Priority queue for storage allocation
- `collections.deque` - Queue for order processing
- `json` - JSON serialization

### Frontend

**No dependencies or npm packages required!**
- Pure HTML, CSS, JavaScript
- No build process needed
- No package.json or node_modules
- Works directly in any modern browser

---

## 💡 Code Implementation Details

### Backend (Python/Flask)

The Flask backend uses in-memory data structures to demonstrate clear algorithm implementations:

```python
# Data storage using Python dictionaries (HashMap)
users_db = {}           # O(1) lookup
crops_db = {}           # O(1) lookup
storage_db = {}         # O(1) lookup
transport_db = {}       # O(1) lookup

# Priority Queue for storage allocation
import heapq
def allocate_optimal_storage(quantity):
    # Uses heapq for O(log n) operations
    pq = []
    for storage in storage_db.values():
        score = calculate_score(storage)
        heapq.heappush(pq, (-score, storage))
    return heapq.heappop(pq)

# Graph for Dijkstra's algorithm
graph = {
    'Lahore': {'Islamabad': 375, 'Multan': 350},
    'Karachi': {'Hyderabad': 165, 'Multan': 890},
    # ... more cities
}

# Queue for order processing
from collections import deque
orders_queue = deque()  # O(1) enqueue/dequeue
```

### Frontend

Pure JavaScript with no frameworks - easy to understand:
- Fetch API for backend communication
- DOM manipulation for dynamic content
- Event listeners for user interactions

---

## 📊 Algorithm Complexity Analysis

### Time Complexity

| Operation | Complexity | Implementation |
|-----------|-----------|----------------|
| User Login | O(1) | HashMap lookup in dict |
| Crop Search | O(1) | HashMap lookup |
| Add Crop | O(1) | HashMap insert |
| Storage Allocation | O(n log n) | Priority queue with heapq |
| Route Finding | O((V+E) log V) | Dijkstra's algorithm |
| Order Enqueue | O(1) | Deque append |
| Order Dequeue | O(1) | Deque popleft |
| View Reports | O(n) | Aggregate operations |

### Space Complexity

| Data Structure | Complexity | Usage |
|----------------|-----------|-------|
| Users HashMap | O(n) | n = number of users |
| Crops HashMap | O(m) | m = number of crops |
| Storage HashMap | O(s) | s = storage centers |
| Transport HashMap | O(t) | t = transport services |
| Graph (Routes) | O(V + E) | V = cities, E = routes |
| Priority Queue | O(n) | Temporary for storage allocation |
| Orders Queue | O(q) | q = pending orders |

### Data Structures Summary

1. **HashMap (dict)** - O(1) average lookup, O(n) space
2. **HashSet (set)** - O(1) lookup for filtering
3. **Priority Queue (heapq)** - O(log n) push/pop
4. **Queue (deque)** - O(1) enqueue/dequeue
5. **Graph (adjacency list)** - O(V+E) space

---

## 🌐 API Documentation

### Base URL
```
http://localhost:5000/api
```

### Endpoints

#### Authentication
- `POST /api/register` - Register new user
  ```json
  {
    "username": "farmer1",
    "password": "pass123",
    "role": "farmer",
    "fullname": "John Doe"
  }
  ```

- `POST /api/login` - User login
  ```json
  {
    "username": "farmer1",
    "password": "pass123"
  }
  ```

#### Crops
- `GET /api/crops` - Get all crops (HashMap traversal - O(n))
- `POST /api/crops` - Add new crop (HashMap insert - O(1))

#### Storage
- `GET /api/storage` - Get all storage centers
- `POST /api/storage/allocate` - Allocate optimal storage using Priority Queue
  ```json
  {
    "quantity": 1000
  }
  ```

#### Transport
- `GET /api/transport` - Get transport services

#### Routes
- `POST /api/routes/shortest` - Calculate shortest path (Dijkstra's)
  ```json
  {
    "start": "Lahore",
    "end": "Karachi"
  }
  ```

#### Orders
- `GET /api/orders` - Get orders queue (FIFO)
- `POST /api/orders` - Add order to queue (O(1) enqueue)

#### System
- `GET /api/stats` - Get system statistics
- `GET /api/info` - Get algorithm information

---

## 📝 License

MIT License - Copyright (c) 2026 AgriConnect Pakistan

---

## 👨‍💻 Developer Notes

### Backend Notes:
- Flask REST API with in-memory data storage
- Clear implementations of data structures (HashMap, Heap, Queue, Graph)
- No database required - all data structures visible in code
- CORS enabled for frontend communication
- Sample data preloaded for testing

### Frontend Notes:
- Vanilla JavaScript (no frameworks)
- Fetch API for REST communication
- Responsive design
- Clean and modern UI
- Easy to customize

### Key Features:
- ✅ **HashMap** - Fast O(1) lookups for all entities
- ✅ **Priority Queue (Heap)** - Optimal storage allocation
- ✅ **Queue (Deque)** - FIFO order processing
- ✅ **Graph + Dijkstra's** - Shortest path routing
- ✅ **REST API** - Clean separation of frontend/backend

---

## 🚀 Next Steps

1. **Install Python** if not already installed
2. **Install Flask**: `pip install -r backend/requirements.txt`
3. **Start backend**: `python backend/app.py`
4. **Open frontend**: Navigate to `frontend/index.html`
5. **Test algorithms**: Try storage allocation and route finding features

---

**Thank you for using AgriConnect Pakistan!**

*Last Updated: January 10, 2026*