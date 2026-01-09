# AgriConnect Pakistan - User Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [User Roles](#user-roles)
3. [Features Guide](#features-guide)
4. [Algorithm Usage](#algorithm-usage)
5. [Tips & Best Practices](#tips--best-practices)

---

## Getting Started

### First Time Users

1. **Install Python and Dependencies**
   ```bash
   # Install Flask
   pip install -r backend/requirements.txt
   ```

2. **Start the Backend Server**
   ```bash
   # Run Flask server
   python backend/app.py
   
   # Server will start on http://localhost:5000
   ```

3. **Open the Application**
   - Open `frontend/index.html` in your browser
   - Or use Python HTTP server:
     ```bash
     cd frontend
     python -m http.server 8000
     ```

4. **Register an Account**
   - Click "Register" on home page
   - Fill in your details
   - Select your role (Farmer, Buyer, Storage Owner, Transport Provider)
   - Submit

5. **Login**
   - Enter username and password
   - Access your personalized dashboard

---

## User Roles

### 👨‍🌾 Farmer

**What You Can Do:**
- List crops for sale
- View storage facilities
- Book cold storage
- Request transport services
- Track your listings
- View earnings reports

**How to List a Crop:**
1. Login to dashboard
2. Click "Crop Management"
3. Click "Add New Crop"
4. Fill in:
   - Crop type (wheat, rice, potato, etc.)
   - Quantity in kg
   - Quality grade (A, B, or C)
   - Price per kg
   - Harvest date
5. Submit

**Tips:**
- Use Grade A for premium quality
- Price competitively
- Update harvest dates accurately
- Check storage availability before listing perishables

### 🛒 Buyer

**What You Can Do:**
- Browse available crops
- Search by crop type
- View farmer information
- Compare prices
- Place orders
- Make payments
- Track deliveries

**How to Browse Crops:**
1. Login to dashboard
2. Click "Crop Management"
3. View all available crops
4. Use search to filter by type
5. Click "View" for details

**Tips:**
- Compare prices from multiple farmers
- Check quality grades
- Verify harvest dates for freshness
- Book transport in advance

### ❄️ Storage Owner

**What You Can Do:**
- Register storage facilities
- Set capacity and temperature
- Monitor utilization
- Manage bookings
- View revenue

**How to Add Storage:**
1. Login to dashboard
2. Click "Cold Storage Management"
3. Click "Add Storage Center"
4. Fill in:
   - Storage name
   - Location/city
   - Total capacity (kg)
   - Temperature (°C)
5. Submit

**Tips:**
- Maintain optimal temperature (4°C for most crops)
- Keep capacity updated
- Respond quickly to booking requests
- Regular maintenance for better ratings

### 🚚 Transport Provider

**What You Can Do:**
- Register transport services
- Set routes and pricing
- Accept delivery requests
- Track deliveries
- Manage fleet

**How to Add Transport:**
1. Login to dashboard
2. Click "Transport Management"
3. Click "Add Transport Service"
4. Fill in:
   - Vehicle type
   - Capacity (kg)
   - Route (e.g., Lahore-Karachi)
   - Price per km
5. Submit

**Tips:**
- Offer competitive pricing
- Maintain regular routes
- Update availability status
- Ensure refrigerated transport for perishables

### 👨‍💼 Admin

**What You Can Do:**
- Manage all users
- Verify accounts
- View system-wide reports
- Monitor transactions
- Access all features

**Default Credentials:**
- Username: `admin`
- Password: `admin123`

---

## Features Guide

### Crop Management

#### Adding Crops
```
Crop Type: Select from dropdown
Quantity: Enter in kilograms
Quality: A (Premium), B (Standard), C (Basic)
Price: Per kilogram in PKR
Harvest Date: When crop was harvested
```

#### Searching Crops
- Use search bar to find specific crop types
- Results show quantity, quality, price, and farmer
- Click "View" for detailed information

### Storage Management

#### Storage Allocation Algorithm

**How it Works:**
1. Backend receives storage allocation request
2. Uses Python's `heapq` (Priority Queue) to evaluate all storage centers
3. Calculates score for each:
   ```
   Score = Available Capacity - (|Temperature - 4°C| × 10)
   ```
4. Uses heap to efficiently select storage with highest score
5. Returns optimal storage (O(n log n) time complexity)

**Using the Algorithm:**
1. Go to "Cold Storage Management"
2. Find "Optimal Storage Allocation" section
3. Enter quantity to store (kg)
4. Click "Find Best Storage"
5. Backend API (`POST /api/storage/allocate`) processes request using Priority Queue
6. Review recommendation

**Understanding Results:**
- **Storage Name & Location**: Where your crop will be stored
- **Temperature**: Current storage temperature
- **Available Capacity**: Space remaining after allocation
- **Algorithm Score**: Higher is better (capacity prioritized, closer to 4°C preferred)
- **Utilization**: Percentage of storage capacity in use

### Transport Management

#### Booking Transport
1. View available transport services
2. Filter by route
3. Check capacity
4. Compare prices
5. Click "Track" to monitor

#### Live Tracking
- Real-time progress updates
- Status indicators:
  - Loading
  - In Transit
  - Near Destination
  - Delivered
- Progress bar showing completion percentage

### Route Finder (Dijkstra's Algorithm)

#### Finding Shortest Route

**How to Use:**
1. Go to "Route Finder"
2. Select start city
3. Select destination city
4. Click "Calculate Shortest Route"
5. Backend API (`POST /api/routes/shortest`) calculates using Dijkstra's algorithm

**Available Cities:**
- Lahore
- Karachi
- Islamabad
- Multan
- Faisalabad
- Peshawar
- Quetta
- Sialkot

**Understanding Results:**
- **Shortest Distance**: Total kilometers via optimal route
- **Estimated Time**: Based on 80 km/h average speed
- **Path**: Complete route with all intermediate cities
- **Algorithm**: Dijkstra's with O((V+E) log V) time complexity
- **Implementation**: Backend uses graph data structure (dict of dicts)

**Example:**
```
Start: Lahore
Destination: Karachi
Result: 
  Distance: 1200 km
  Time: 15 hours
  Path: Lahore → Multan → Karachi
  Algorithm: Dijkstra's Shortest Path
```

### Payment System

#### Making Payments

**Payment Methods:**
1. **Cash**
   - Select cash option
   - Amount recorded in system
   - Pay on delivery

2. **EasyPaisa**
   - Select EasyPaisa
   - Redirected to payment gateway
   - Enter mobile number and PIN
   - Confirm transaction

3. **JazzCash**
   - Select JazzCash
   - Redirected to payment gateway
   - Enter mobile number and MPIN
   - Confirm transaction

4. **Bank Transfer**
   - Select bank transfer
   - Redirected to banking portal
   - Enter account details
   - Complete transfer

### Reports & Analytics

#### Viewing Reports

**Available Reports:**
1. **User Statistics**
   - Total users by role
   - Registration trends
   - Active vs inactive

2. **Crop Summary**
   - Total listings
   - Quantity available
   - Average prices
   - Most listed crops

3. **Storage Utilization**
   - Total capacity
   - Available space
   - Utilization percentage
   - By location

4. **Transport Statistics**
   - Total services
   - Active deliveries
   - Most used routes
   - Capacity analysis

5. **Financial Summary**
   - Total transactions
   - Revenue breakdown
   - Payment methods distribution

6. **Algorithm Performance**
   - Time complexity metrics
   - Operation counts
   - Efficiency statistics

#### Exporting Reports

1. Click "Export as PDF" for formatted report
2. Click "Export as CSV" for data analysis
3. Files download to your computer

---

## Algorithm Usage

### Priority Queue Storage Allocation

**When to Use:**
- Need to find best storage quickly
- Multiple options available
- Want optimal temperature match

**Algorithm Steps:**
1. Backend receives request via REST API
2. Retrieves all storage centers from HashMap (dict)
3. Filters by sufficient capacity
4. Calculates score for each storage
5. Uses Python's `heapq` to build priority queue
6. Returns highest scoring option in O(n log n) time

**Time Complexity:** O(n log n) where n = number of storage centers

**Data Structure:** Python's `heapq` module (min-heap, negated for max behavior)

### Dijkstra's Shortest Path

**When to Use:**
- Planning transport routes
- Comparing delivery options
- Estimating travel time and distance

**Algorithm Steps:**
1. Backend receives start and destination cities
2. Initializes distances to infinity (except start = 0)
3. Uses priority queue to select minimum distance node
4. Relaxes edges to find shortest paths
5. Reconstructs path from start to destination
6. Returns distance, time, and complete path

**Time Complexity:** O((V+E) log V) where V = cities, E = routes

**Data Structure:** Graph as adjacency list (nested dict in Python)

### HashMap Operations

**Used Throughout:**
- User authentication: O(1) lookup
- Crop management: O(1) insert/retrieve
- Storage lookup: O(1) access
- Transport services: O(1) operations

**Implementation:** Python's built-in `dict` type

---

## Tips & Best Practices

### For All Users

✅ **DO:**
- Update your profile regularly
- Respond promptly to requests
- Maintain accurate listings
- Verify information before submitting
- Use search and filter features
- Check reports for insights

❌ **DON'T:**
- Share your password
- List unavailable items
- Ignore system notifications
- Provide false information
- Skip verification steps

### Data Management

- **Backup Data**: Export reports regularly
- **Clean Old Listings**: Remove sold items
- **Update Prices**: Keep market competitive
- **Monitor Capacity**: Avoid overbooking

### Security

- **Strong Passwords**: Use mix of letters, numbers, symbols
- **Logout**: Always logout after use
- **Verify Users**: Check ratings and history
- **Secure Payments**: Use digital payment options

### Performance

- **Use Filters**: Narrow search results
- **Batch Operations**: Add multiple items together
- **Regular Updates**: Keep information current
- **Monitor Reports**: Track trends and patterns

---

## Keyboard Shortcuts

### Web Frontend
- `Ctrl+F`: Search on page
- `Tab`: Navigate forms
- `Enter`: Submit forms
- `Esc`: Close modals

---

## Common Tasks

### Task 1: List and Sell a Crop
1. Login as Farmer
2. Add crop with details
3. Wait for buyer interest
4. Allocate to storage if needed
5. Arrange transport
6. Receive payment

### Task 2: Buy Crops
1. Login as Buyer
2. Browse available crops
3. Compare prices and quality
4. Place order
5. Make payment
6. Track delivery

### Task 3: Optimize Storage
1. Login as Storage Owner
2. Add your facility
3. Set optimal temperature
4. Monitor utilization
5. Accept bookings
6. Track revenue

### Task 4: Find Best Route
1. Go to Route Finder
2. Select cities
3. Run algorithm
4. Review shortest path
5. Plan transport accordingly

---

## FAQ

**Q: Can I have multiple roles?**
A: No, each account has one role. Register separate accounts for different roles.

**Q: How do I change my password?**
A: Currently in development. Contact admin for password reset.

**Q: What's the best temperature for storage?**
A: 4°C is optimal for most crops. The algorithm prioritizes storage closest to this temperature.

**Q: How accurate is the route finder?**
A: Uses actual city distances with Dijkstra's algorithm. Add 10-20% for real travel due to traffic and road conditions.

**Q: Is data persistent?**
A: Currently uses in-memory storage. Data resets when server restarts. For production, integrate with a database.

**Q: How do I view the data structures in action?**
A: Check the Flask backend code in `backend/app.py` - all data structures (dict, heapq, deque, graph) are clearly visible.

**Q: Can I test the algorithms?**
A: Yes! Use the storage allocation feature and route finder. The backend will execute the Priority Queue and Dijkstra's algorithms.

---

## Glossary

- **PKR**: Pakistani Rupee
- **Quality Grade**: A (Premium), B (Standard), C (Basic)
- **Utilization**: Percentage of storage capacity in use
- **Algorithm Score**: Calculated value for optimal selection
- **Time Complexity**: Measure of algorithm efficiency

---

## Support

For help:
- Review this user guide
- Check FAQ section
- Contact system admin
- Report bugs via feedback form

---

**Happy farming with AgriConnect Pakistan!**
