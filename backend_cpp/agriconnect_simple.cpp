#include <iostream>
#include <string>
#include <iomanip>
#include <functional>
using namespace std;

// Forward declarations for JSON handler
void saveAllData();
void loadAllData();

const int HASH_TABLE_SIZE = 101;
const int MAX_CITIES = 8;
const int MAX_HEAP_SIZE = 100;

//data structures

struct SimpleDate {
    int day;
    int month;
    int year;
};

// User node for hash table + linked list
struct User {
    int userId;
    string username;
    string password;
    string role;
    bool active;
    User* nextInBucket;
    User* nextInList;
};

// Crop node for BST (sorted by price)
struct Crop {
    int cropId;
    int farmerId;
    string cropType;
    int quantity;
    int storageQuantity;
    string quality;
    int pricePerKg;
    bool available;
    SimpleDate dateAdded;
    SimpleDate expiryDate;
    string storageCenter;
    Crop* left;
    Crop* right;
};

// Linked list nodes
struct Order {
    int orderId;
    int cropId;
    int buyerId;
    int farmerId;
    int quantity;
    bool farmerApproved;
    bool paid;
    bool delivered;
    SimpleDate orderDate;
    SimpleDate approvalDate;
    SimpleDate paymentDate;
    Order* next;
};

struct TransportRequest {
    int requestId;
    int cropIds[50];
    int quantities[50];
    int cropCount;
    int weight;
    int distance;
    int budget;
    string organization;
    bool accepted;
    bool rejected;
    bool completed;
    int requesterId;
    SimpleDate requestDate;
    SimpleDate completionDate;
    TransportRequest* next;
};

struct StorageRequest {
    int requestId;
    int cropId;
    int ownerId;
    string cropName;
    string requesterName;
    int quantity;
    int budget;
    int pricePerKg;
    string organization;
    bool accepted;
    bool rejected;
    bool released;
    int requesterId;
    SimpleDate registrationDate;
    SimpleDate releaseDate;
    SimpleDate approvalDate;
    StorageRequest* next;
};

struct Vehicle {
    int vehicleId;
    int capacity;
    bool available;
    string type;
    string organization;
    Vehicle* next;
};

struct StorageCenter {
    string organization;
    int totalCapacity;
    int availableCapacity;
    float temperature;
    string location;
    int pricePerKg;
    SimpleDate registrationDate;
    StorageCenter* next;
};

// Graph: Adjacency list for cities
struct EdgeNode {
    int cityIdx;
    int distance;
    EdgeNode* next;
};

struct City {
    string name;
    EdgeNode* adjList;
};

// Min-Heap for Dijkstra
struct HeapNode {
    int cityIdx;
    int distance;
};


User* userHashTable[HASH_TABLE_SIZE] = { nullptr };
User* userListHead = nullptr;
Crop* cropBSTRoot = nullptr;
Order* orderHead = nullptr;
TransportRequest* transportHead = nullptr;
StorageRequest* storageHead = nullptr;
Vehicle* vehicleHead = nullptr;
StorageCenter* storageCenterHead = nullptr;
City cities[MAX_CITIES];

int userCount = 0, cropCount = 0, orderCount = 0;
int transportReqCount = 0, storageReqCount = 0;
int vehicleCount = 0, storageCenterCount = 0, cityCount = 0;
int currentUserId = -1;


int maxInt(int a, int b) { return (a > b) ? a : b; }

string twoDigit(int n) {
    if (n < 10) return "0" + to_string(n);
    return to_string(n);
}

SimpleDate getCurrentDate() { return { 1, 1, 2026 }; }

string toLower(string s) {
    string result = s;
    for (int i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32;
        }
    }
    return result;
}

SimpleDate readDate() {
    SimpleDate d;
    cout << "Enter date (day month year): ";
    cin >> d.day >> d.month >> d.year;
    return d;
}

string formatDate(SimpleDate d) {
    return twoDigit(d.day) + "-" + twoDigit(d.month) + "-" + to_string(d.year);
}

int hashUsername(string username) {
    int h = 0;
    for (int i = 0; i < username.length(); i++)
        h = (h * 31 + username[i]) % HASH_TABLE_SIZE;
    return h;
}

int hashID(string key) {
    int h = 0;
    for (int i = 0; i < key.length(); i++)
        h = (h * 31 + key[i]) % 100000;
    return h;
}

//hash table

void insertUser(User* newUser) {
    int idx = hashUsername(newUser->username);
    newUser->nextInBucket = userHashTable[idx];
    userHashTable[idx] = newUser;

    newUser->nextInList = userListHead;
    userListHead = newUser;
    userCount++;
}

User* findUserByUsername(string username) {
    int idx = hashUsername(username);
    User* curr = userHashTable[idx];
    while (curr != nullptr) {
        if (curr->username == username) return curr;
        curr = curr->nextInBucket;
    }
    return nullptr;
}

User* findUserById(int userId) {
    User* curr = userListHead;
    while (curr != nullptr) {
        if (curr->userId == userId) return curr;
        curr = curr->nextInList;
    }
    return nullptr;
}

//BST

Crop* insertCropBST(Crop* root, Crop* newCrop) {
    if (root == nullptr) return newCrop;

    if (newCrop->pricePerKg < root->pricePerKg)
        root->left = insertCropBST(root->left, newCrop);
    else
        root->right = insertCropBST(root->right, newCrop);

    return root;
}

void inOrderTraversal(Crop* root) {
    if (root == nullptr) return;
    inOrderTraversal(root->left);
    if (root->available) {
        User* farmer = findUserById(root->farmerId);
        string farmerName = (farmer != nullptr) ? farmer->username : "Unknown";
        cout << setw(8) << root->cropId << setw(12) << root->cropType
            << setw(8) << root->quality << setw(10) << root->pricePerKg
            << setw(12) << root->quantity << setw(15) << farmerName << "\n";
    }
    inOrderTraversal(root->right);
}

Crop* findCropById(Crop* root, int cropId) {
    if (root == nullptr) return nullptr;
    if (root->cropId == cropId) return root;

    Crop* left = findCropById(root->left, cropId);
    if (left != nullptr) return left;
    return findCropById(root->right, cropId);
}

int countCropsForFarmer(Crop* root, int farmerId) {
    if (root == nullptr) return 0;
    int left = countCropsForFarmer(root->left, farmerId);
    int self = (root->farmerId == farmerId) ? 1 : 0;
    int right = countCropsForFarmer(root->right, farmerId);
    return left + self + right;
}

void printFarmerCrops(Crop* root, int farmerId) {
    if (root == nullptr) return;
    printFarmerCrops(root->left, farmerId);
    if (root->farmerId == farmerId) {
        cout << root->cropId << "\t" << root->cropType << "\t"
            << root->quantity << "\t\t" << root->quality << "\t"
            << root->pricePerKg << "\t" << formatDate(root->dateAdded) << "\n";
    }
    printFarmerCrops(root->right, farmerId);
}

void printCropsByTypeAndBudget(Crop* root, const string& targetCrop, int maxBudget, bool& found) {
    if (root == nullptr) return;
    printCropsByTypeAndBudget(root->left, targetCrop, maxBudget, found);
    if (root->available && toLower(root->cropType) == toLower(targetCrop) && root->pricePerKg <= maxBudget) {
        found = true;
        User* farmer = findUserById(root->farmerId);
        string farmerName = (farmer != nullptr) ? farmer->username : "Unknown";
        cout << setw(8) << root->cropId << setw(8) << root->quality
            << setw(10) << root->pricePerKg << setw(12) << root->quantity
            << setw(15) << farmerName << "\n";
    }
    printCropsByTypeAndBudget(root->right, targetCrop, maxBudget, found);
}

//linked list

void addOrder(Order* newOrder) {
    newOrder->next = orderHead;
    orderHead = newOrder;
    orderCount++;
}

void addTransportRequest(TransportRequest* req) {
    req->next = transportHead;
    transportHead = req;
    transportReqCount++;
}

void addStorageRequest(StorageRequest* req) {
    req->next = storageHead;
    storageHead = req;
    storageReqCount++;
}

void addVehicle(Vehicle* v) {
    v->next = vehicleHead;
    vehicleHead = v;
    vehicleCount++;
}

void addStorageCenter(StorageCenter* sc) {
    sc->next = storageCenterHead;
    storageCenterHead = sc;
    storageCenterCount++;
}

// graph(adjacency list)

void addEdge(int cityIdx, int neighborIdx, int distance) {
    EdgeNode* newEdge = new EdgeNode;
    newEdge->cityIdx = neighborIdx;
    newEdge->distance = distance;
    newEdge->next = cities[cityIdx].adjList;
    cities[cityIdx].adjList = newEdge;
}

// min heap

struct MinHeap {
    HeapNode heap[MAX_HEAP_SIZE];
    int size;
    int position[MAX_CITIES];
};

void initHeap(MinHeap* h) {
    h->size = 0;
    for (int i = 0; i < MAX_CITIES; i++)
        h->position[i] = -1;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->heap[idx].distance < h->heap[parent].distance) {
            h->position[h->heap[idx].cityIdx] = parent;
            h->position[h->heap[parent].cityIdx] = idx;
            swapHeapNodes(&h->heap[idx], &h->heap[parent]);
            idx = parent;
        }
        else break;
    }
}

void heapifyDown(MinHeap* h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->heap[left].distance < h->heap[smallest].distance)
        smallest = left;
    if (right < h->size && h->heap[right].distance < h->heap[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        h->position[h->heap[smallest].cityIdx] = idx;
        h->position[h->heap[idx].cityIdx] = smallest;
        swapHeapNodes(&h->heap[smallest], &h->heap[idx]);
        heapifyDown(h, smallest);
    }
}

void insertHeap(MinHeap* h, int cityIdx, int distance) {
    HeapNode node = { cityIdx, distance };
    h->heap[h->size] = node;
    h->position[cityIdx] = h->size;
    heapifyUp(h, h->size);
    h->size++;
}

HeapNode extractMin(MinHeap* h) {
    HeapNode minNode = h->heap[0];
    h->position[minNode.cityIdx] = -1;
    h->size--;

    if (h->size > 0) {
        h->heap[0] = h->heap[h->size];
        h->position[h->heap[0].cityIdx] = 0;
        heapifyDown(h, 0);
    }

    return minNode;
}

void decreaseKey(MinHeap* h, int cityIdx, int newDist) {
    int idx = h->position[cityIdx];
    if (idx == -1) return;
    h->heap[idx].distance = newDist;
    heapifyUp(h, idx);
}

bool isInHeap(MinHeap* h, int cityIdx) {
    return h->position[cityIdx] != -1 && h->position[cityIdx] < h->size;
}

// ==================== ALGORITHMS ==================== 

//Greedy Vehicle Assignment
Vehicle* assignVehicle(int weight, string organization) {
    Vehicle* curr = vehicleHead;
    while (curr != nullptr) {
        if (curr->organization == organization && curr->available && curr->capacity >= weight) {
            curr->available = false;
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

//0/1 Knapsack DP
int knapsackStorageAllocation(int capacity, int weights[], int values[], int n, bool selected[]) {
    const int MAX_CAP_DP = 200;
    int scale = (capacity > MAX_CAP_DP) ? (capacity / MAX_CAP_DP) : 1;
    int capN = (capacity / scale);
    if (capN > MAX_CAP_DP) capN = MAX_CAP_DP;

    int weightsN[60];
    for (int i = 0; i < n; i++) {
        int w = weights[i] / scale;
        if (w < 1) w = 1;
        if (w > capN) w = capN;
        weightsN[i] = w;
    }

    int dp[61][MAX_CAP_DP + 1];
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capN; w++) {
            if (i == 0 || w == 0) dp[i][w] = 0;
            else if (weightsN[i - 1] <= w)
                dp[i][w] = maxInt(values[i - 1] + dp[i - 1][w - weightsN[i - 1]], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    int w = capN;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected[i - 1] = true;
            w -= weightsN[i - 1];
        }
        else {
            selected[i - 1] = false;
        }
    }

    return dp[n][capN];
}

//Dijkstra with Min-Heap
int dijkstraShortestPath(int startIdx, int endIdx, int path[], int& pathLength) {
    const int INF = 999999;
    int dist[MAX_CITIES];
    int parent[MAX_CITIES];
    MinHeap heap;
    initHeap(&heap);

    for (int i = 0; i < cityCount; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[startIdx] = 0;
    insertHeap(&heap, startIdx, 0);

    while (heap.size > 0) {
        HeapNode minNode = extractMin(&heap);
        int u = minNode.cityIdx;

        if (minNode.distance > dist[u]) continue;

        EdgeNode* edge = cities[u].adjList;
        while (edge != nullptr) {
            int v = edge->cityIdx;
            int newDist = dist[u] + edge->distance;

            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;

                if (isInHeap(&heap, v))
                    decreaseKey(&heap, v, newDist);
                else
                    insertHeap(&heap, v, newDist);
            }
            edge = edge->next;
        }
    }

    pathLength = 0;
    if (dist[endIdx] != INF) {
        int curr = endIdx;
        int tempPath[MAX_CITIES];
        int tempLen = 0;

        while (curr != -1) {
            tempPath[tempLen++] = curr;
            curr = parent[curr];
        }

        for (int i = 0; i < tempLen; i++)
            path[i] = tempPath[tempLen - 1 - i];
        pathLength = tempLen;
    }

    return dist[endIdx];
}

/* ==================== USER MANAGEMENT ==================== */

void registerUser() {
    string username, password, role;
    cout << "\n=== User Registration ===\n";
    cout << "Username: "; cin >> username;

    if (findUserByUsername(username) != nullptr) {
        cout << "Username already exists!\n";
        return;
    }

    cout << "Password: "; cin >> password;
    cout << "Role (farmer/buyer/storage_owner/transport_provider): ";
    cin >> role;

    User* newUser = new User;
    newUser->userId = hashID(username + to_string(userCount));
    newUser->username = username;
    newUser->password = password;
    newUser->role = role;
    newUser->active = true;
    newUser->nextInBucket = nullptr;
    newUser->nextInList = nullptr;

    insertUser(newUser);
    cout << "Registration successful! User ID: " << newUser->userId << "\n";
    saveAllData();  // Save to JSON after registration
}

bool loginUser() {
    string username, password;
    cout << "\n=== Login ===\n";
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;

    User* user = findUserByUsername(username);
    if (user != nullptr && user->password == password) {
        currentUserId = user->userId;
        cout << "Login successful! Welcome " << username << "\n";
        return true;
    }

    cout << "Invalid credentials!\n";
    return false;
}

void viewAllUsers() {
    cout << "\n=== All Users ===\n";
    int count = 1;
    User* curr = userListHead;
    while (curr != nullptr) {
        cout << count++ << ". " << curr->username << " (Role: " << curr->role << ")\n";
        curr = curr->nextInList;
    }
}

/* ==================== CROP MANAGEMENT ==================== */

void addCrop() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "farmer") {
        cout << "Only farmers can add crops!\n";
        return;
    }

    string cropType, quality;
    int quantity, price;

    cout << "\n=== Add New Crop ===\n";
    cout << "Crop Type (Wheat/Rice/Mango/etc): "; cin >> cropType;
    cout << "Quantity (kg): "; cin >> quantity;
    cout << "Quality (A/B/C): "; cin >> quality;
    cout << "Price per kg: "; cin >> price;

    cout << "Expected Expiry Date: ";
    SimpleDate expiryDate = readDate();

    Crop* newCrop = new Crop;
    newCrop->cropId = hashID(cropType + to_string(cropCount));
    newCrop->farmerId = currentUserId;
    newCrop->cropType = toLower(cropType);
    newCrop->quantity = quantity;
    newCrop->storageQuantity = 0;
    newCrop->quality = quality;
    newCrop->pricePerKg = price;
    newCrop->available = true;
    newCrop->expiryDate = expiryDate;
    newCrop->dateAdded = getCurrentDate();
    newCrop->storageCenter = "";
    newCrop->left = newCrop->right = nullptr;

    cropBSTRoot = insertCropBST(cropBSTRoot, newCrop);
    cropCount++;

    cout << "Crop added successfully! Crop ID: " << newCrop->cropId << "\n";
    saveAllData();  // Save to JSON after adding crop
}

void viewAllCrops() {
    cout << "\n=== All Available Crops (Sorted by Price) ===\n";
    cout << setw(8) << "CropID" << setw(12) << "Type" << setw(8) << "Quality"
        << setw(10) << "Price/kg" << setw(12) << "Quantity" << setw(15) << "Farmer\n";
    cout << "==================================================================================\n";
    inOrderTraversal(cropBSTRoot);
}

void compareCropPrices() {
    string targetCrop;
    int maxBudget;
    cout << "\n=== Compare Crop Prices ===\n";
    cout << "Enter crop type to compare (e.g., Wheat, Rice, Mango): ";
    cin >> targetCrop;
    cout << "Enter your max budget per kg (Rs): ";
    cin >> maxBudget;

    cout << "\n=== " << targetCrop << " Listings (Filtered by Budget) ===\n";
    cout << setw(8) << "CropID" << setw(8) << "Quality" << setw(10) << "Price/kg"
        << setw(12) << "Quantity" << setw(15) << "Farmer\n";
    cout << "==================================================================================\n";
    bool found = false;
    printCropsByTypeAndBudget(cropBSTRoot, targetCrop, maxBudget, found);
    if (!found) {
        cout << "No listings found for " << targetCrop << " within budget " << maxBudget << "\n";
    }
}

/* ==================== ORDER MANAGEMENT ==================== */

void requestCrop() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "buyer") {
        cout << "Only buyers can request crops!\n";
        return;
    }

    int cropId, quantity;
    cout << "\n=== Request Crop ===\n";
    viewAllCrops();
    cout << "Enter Crop ID: "; cin >> cropId;
    cout << "Enter Quantity (kg): "; cin >> quantity;

    Crop* crop = findCropById(cropBSTRoot, cropId);
    if (crop == nullptr || !crop->available) {
        cout << "Crop not found or unavailable!\n";
        return;
    }

    if (crop->quantity < quantity) {
        cout << "Insufficient quantity available!\n";
        return;
    }

    Order* newOrder = new Order;
    newOrder->orderId = orderCount + 1;
    newOrder->cropId = cropId;
    newOrder->buyerId = currentUserId;
    newOrder->farmerId = crop->farmerId;
    newOrder->quantity = quantity;
    newOrder->farmerApproved = false;
    newOrder->paid = false;
    newOrder->delivered = false;
    newOrder->orderDate = getCurrentDate();
    newOrder->next = nullptr;

    addOrder(newOrder);
    cout << "Crop request sent to farmer! Order ID: " << newOrder->orderId << "\n";
    cout << "Awaiting farmer approval before payment.\n";
    saveAllData();  // Save to JSON after creating order
}

void viewMyOrders() {
    cout << "\n=== My Orders ===\n";
    cout << setw(8) << "OrderID" << setw(8) << "CropID" << setw(10) << "Quantity"
        << setw(12) << "Approved" << setw(8) << "Paid" << setw(12) << "Delivered\n";
    cout << "==================================================================================\n";

    Order* curr = orderHead;
    while (curr != nullptr) {
        if (curr->buyerId == currentUserId) {
            cout << setw(8) << curr->orderId << setw(8) << curr->cropId
                << setw(10) << curr->quantity << setw(12) << (curr->farmerApproved ? "Yes" : "No")
                << setw(8) << (curr->paid ? "Yes" : "No")
                << setw(12) << (curr->delivered ? "Yes" : "No") << "\n";
        }
        curr = curr->next;
    }
}

void payForOrder() {
    int orderId;
    cout << "\n=== Pay for Order ===\n";
    viewMyOrders();
    cout << "Enter Order ID to pay: "; cin >> orderId;

    Order* curr = orderHead;
    while (curr != nullptr) {
        if (curr->orderId == orderId && curr->buyerId == currentUserId) {
            if (!curr->farmerApproved) {
                cout << "Order not approved by farmer yet! Cannot pay.\n";
                return;
            }
            if (curr->paid) {
                cout << "Order already paid!\n";
            }
            else {
                curr->paid = true;
                curr->paymentDate = getCurrentDate();
                cout << "Payment successful!\n";
                saveAllData();  // Save to JSON after payment
            }
            return;
        }
        curr = curr->next;
    }

    cout << "Order not found!\n";
}

void viewCropRequests() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "farmer") {
        cout << "Only farmers can view crop requests!\n";
        return;
    }

    cout << "\n=== Crop Requests from Buyers ===\n";
    cout << setw(8) << "OrderID" << setw(8) << "CropID" << setw(10) << "Quantity"
        << setw(12) << "BuyerID" << setw(12) << "Status\n";
    cout << "=======================================================\n";

    Order* curr = orderHead;
    bool found = false;
    while (curr != nullptr) {
        if (curr->farmerId == currentUserId && !curr->farmerApproved) {
            cout << setw(8) << curr->orderId << setw(8) << curr->cropId
                << setw(10) << curr->quantity << setw(12) << curr->buyerId
                << setw(12) << "Pending\n";
            found = true;
        }
        curr = curr->next;
    }

    if (!found) {
        cout << "No pending crop requests.\n";
    }
}

void approveCropRequest() {
    int orderId;
    cout << "\n=== Approve/Reject Crop Request ===\n";
    viewCropRequests();

    cout << "\nEnter Order ID: "; cin >> orderId;
    cout << "Approve? (1=Yes / 0=No): "; int approve; cin >> approve;

    Order* curr = orderHead;
    while (curr != nullptr) {
        if (curr->orderId == orderId && curr->farmerId == currentUserId && !curr->farmerApproved) {
            if (approve == 1) {
                Crop* crop = findCropById(cropBSTRoot, curr->cropId);
                if (crop != nullptr && crop->quantity >= curr->quantity) {
                    curr->farmerApproved = true;
                    curr->approvalDate = getCurrentDate();
                    crop->quantity -= curr->quantity;
                    if (crop->quantity == 0) crop->available = false;
                    cout << "Request approved! Buyer can now pay.\n";
                }
                else {
                    cout << "Insufficient crop quantity!\n";
                }
            }
            else {
                cout << "Request rejected.\n";
            }
            saveAllData();  // Save to JSON after approving/rejecting order
            return;
        }
        curr = curr->next;
    }

    cout << "Request not found!\n";
}

/* ==================== TRANSPORT MANAGEMENT ==================== */

void requestTransport() {
    User* user = findUserById(currentUserId);
    if (user == nullptr) return;

    cout << "\n=== Request Transport ===\n";

    if (user->role == "farmer") {

        cout << "\n=== Your Crops Accepted by Storage Centers ===\n";
        cout << setw(8) << "CropID" << setw(12) << "Type" << setw(12) << "Qty Stored"
            << setw(15) << "Storage Center\n";
        cout << "==================================================================================\n";


        bool foundStored = false;
        StorageRequest* sr = storageHead;

        while (sr != nullptr) {

            if (sr->accepted && !sr->released && sr->requesterId == currentUserId) {

                Crop* crop = findCropById(cropBSTRoot, sr->cropId);


                if (crop != nullptr && crop->farmerId == currentUserId) {
                    cout << setw(8) << crop->cropId << setw(12) << crop->cropType
                        << setw(12) << sr->quantity << setw(15) << sr->organization << "\n";
                    foundStored = true;
                }
            }
            sr = sr->next;
        }

        if (!foundStored) {
            cout << "No crops with accepted storage requests to transport!\n";
            return;
        }

        cout << "\n=== Select Crops to Transport ===\n";
        cout << "Enter crop IDs separated by space: ";

        int tempCropIds[50];
        int tempCount = 0;


        string line;
        cin.ignore();
        getline(cin, line);


        int pos = 0;
        while (pos < line.length() && tempCount < 50) {

            while (pos < line.length() && line[pos] == ' ') pos++;
            if (pos >= line.length()) break;

 
            int num = 0;
            while (pos < line.length() && line[pos] >= '0' && line[pos] <= '9') {
                num = num * 10 + (line[pos] - '0');
                pos++;
            }
            tempCropIds[tempCount++] = num;
        }

        if (tempCount == 0) {
            cout << "No crops selected!\n";
            return;
        }


        int cropIds[50];
        int quantities[50];
        int validCount = 0;
        int totalWeight = 0;

        for (int i = 0; i < tempCount; i++) {
            Crop* crop = findCropById(cropBSTRoot, tempCropIds[i]);
            if (crop == nullptr || crop->farmerId != currentUserId || crop->storageQuantity == 0) {
                cout << "Crop ID " << tempCropIds[i] << " invalid or not in storage - skipped\n";
                continue;
            }

            cout << "Quantity for Crop " << tempCropIds[i] << " (max " << crop->storageQuantity << " kg): ";
            int qty; cin >> qty;

            if (qty > crop->storageQuantity || qty <= 0) {
                cout << "Invalid quantity - skipped\n";
                continue;
            }

            cropIds[validCount] = tempCropIds[i];
            quantities[validCount] = qty;
            totalWeight += qty;
            validCount++;
        }

        if (validCount == 0) {
            cout << "No valid crops selected!\n";
            return;
        }

        int budget;
        string organization;
        cout << "\nTotal weight: " << totalWeight << " kg\n";
        cout << "Budget (Rs): "; cin >> budget;
        cout << "Transport Organization (FastMove/AgriTrans/GreenWay): "; cin >> organization;


        TransportRequest* req = new TransportRequest;
        req->requestId = transportReqCount + 1;
        req->cropCount = validCount;
        req->weight = totalWeight;
        for (int i = 0; i < validCount; i++) {
            req->cropIds[i] = cropIds[i];
            req->quantities[i] = quantities[i];
        }
        req->budget = budget;
        req->organization = organization;
        req->accepted = false;
        req->rejected = false;
        req->completed = false;
        req->requesterId = currentUserId;
        req->requestDate = getCurrentDate();
        req->next = nullptr;

        addTransportRequest(req);
        cout << "\nTransport request created! Request ID: " << req->requestId << "\n";
        cout << "Total crops: " << req->cropCount << "\n";
        cout << "Total weight: " << req->weight << " kg\n";
        saveAllData();  // Save to JSON after transport request

    }
    else if (user->role == "buyer") {

        int orderId, budget;
        string organization;

        cout << "\n=== My Orders ===\n";
        viewMyOrders();

        cout << "\nEnter Order ID: "; cin >> orderId;

        Order* order = orderHead;
        bool found = false;
        while (order != nullptr) {
            if (order->orderId == orderId && order->buyerId == currentUserId) {
                found = true;
                break;
            }
            order = order->next;
        }

        if (!found) {
            cout << "Invalid order ID or not your order!\n";
            return;
        }

        if (!order->farmerApproved) {
            cout << "Cannot request transport: Order not yet approved by farmer!\n";
            return;
        }

        if (!order->paid) {
            cout << "Cannot request transport: Payment not completed!\n";
            return;
        }

        Crop* crop = findCropById(cropBSTRoot, order->cropId);
        cout << "Order: " << crop->cropType << " (" << order->quantity << " kg)\n";

        cout << "Budget (Rs): "; cin >> budget;
        cout << "Organization (FastMove/AgriTrans/GreenWay): "; cin >> organization;

        TransportRequest* req = new TransportRequest;
        req->requestId = transportReqCount + 1;
        req->cropIds[0] = order->cropId;
        req->quantities[0] = order->quantity;
        req->cropCount = 1;
        req->weight = order->quantity;
        req->budget = budget;
        req->organization = organization;
        req->accepted = false;
        req->rejected = false;
        req->completed = false;
        req->requesterId = currentUserId;
        req->requestDate = getCurrentDate();
        req->next = nullptr;

        addTransportRequest(req);
        cout << "\nTransport request created! Request ID: " << req->requestId << "\n";
    }
    else {
        cout << "Only farmers and buyers can request transport!\n";
        return;
    }
}

void viewTransportRequests() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "transport_provider") {
        cout << "Only transport providers can view requests!\n";
        return;
    }

    string org = user->username; 

    cout << "\n=== Transport Requests for " << org << " ===\n";
    cout << "ReqID\tCropCount\tWeight\tBudget\tRequester\tStatus\n";
    cout << "--------------------------------------------------------------\n";

    TransportRequest* curr = transportHead;
    while (curr != nullptr) {
        if (curr->organization == org) {
            User* requester = findUserById(curr->requesterId);
            string requesterName = (requester != nullptr) ? requester->username : "Unknown";
            string status;
            if (curr->rejected) status = "Rejected";
            else if (curr->completed) status = "Completed";
            else if (curr->accepted) status = "In Progress";
            else status = "Pending";

            cout << curr->requestId << "\t" << curr->cropCount << "\t"
                << curr->weight << "\t"
                << curr->budget << "\t" << requesterName << "\t"
                << status << "\n";
        }
        curr = curr->next;
    }
}

void acceptTransportRequest() {
    int reqId;
    cout << "\n=== Accept Transport Request ===\n";
    viewTransportRequests();
    cout << "Enter Request ID to accept: "; cin >> reqId;

    User* user = findUserById(currentUserId);
    if (user == nullptr) return;

    string org = user->username;

    TransportRequest* curr = transportHead;
    while (curr != nullptr) {
        if (curr->requestId == reqId && !curr->accepted) {
            Vehicle* vehicle = assignVehicle(curr->weight, org);

            if (vehicle != nullptr) {
                curr->accepted = true;
                cout << "Request accepted! Vehicle " << vehicle->vehicleId
                    << " assigned (Capacity: " << vehicle->capacity << " kg)\n";
            }
            else {
                cout << "No vehicle available with sufficient capacity!\n";
            }
            return;
        }
        curr = curr->next;
    }

    cout << "Request not found or already accepted!\n";
}

void rejectTransportRequest() {
    int reqId;
    cout << "\n=== Reject Transport Request ===\n";
    viewTransportRequests();
    cout << "Enter Request ID to reject: "; cin >> reqId;

    User* user = findUserById(currentUserId);
    if (user == nullptr) return;

    string org = user->username;

    TransportRequest* curr = transportHead;
    while (curr != nullptr) {
        if (curr->requestId == reqId && curr->organization == org && !curr->accepted && !curr->rejected) {
            curr->rejected = true;
            cout << "Request rejected successfully!\n";
            return;
        }
        curr = curr->next;
    }

    cout << "Request not found, already processed, or not for your organization!\n";
}

void completeDelivery() {
    int reqId;
    cout << "\n=== Complete Delivery ===\n";
    cout << "Enter Request ID: "; cin >> reqId;

    TransportRequest* tr = transportHead;
    while (tr != nullptr) {
        if (tr->requestId == reqId && tr->accepted) {
            tr->completed = true;


            for (int i = 0; i < tr->cropCount; i++) {
                Order* ord = orderHead;
                while (ord != nullptr) {
                    if (ord->cropId == tr->cropIds[i]) {
                        ord->delivered = true;
                        break;
                    }
                    ord = ord->next;
                }
            }

            cout << "Delivery completed successfully!\n";
            return;
        }
        tr = tr->next;
    }

    cout << "Request not found or not accepted yet!\n";
}


void registerVehicle() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "transport_provider") {
        cout << "Only transport providers can register vehicles!\n";
        return;
    }

    string org = user->username;
    int capacity;
    string type;

    cout << "\n=== Register New Vehicle ===\n";
    cout << "Vehicle Type (Van/Truck/Large Truck/Container/Pickup): ";
    cin >> type;
    cout << "Capacity (kg): ";
    cin >> capacity;

    Vehicle* v = new Vehicle;
    v->vehicleId = vehicleCount + 1;
    v->capacity = capacity;
    v->available = true;
    v->type = type;
    v->organization = org;
    v->next = nullptr;

    addVehicle(v);
    cout << "Vehicle registered successfully! Vehicle ID: " << v->vehicleId << "\n";
    cout << "Organization: " << org << "\n";
}

void viewMyVehicles() {
    User* user = findUserById(currentUserId);
    if (user == nullptr) return;

    string org = user->username;

    cout << "\n=== My Vehicles ===\n";
    cout << "VehicleID\tType\t\tCapacity\tStatus\n";
    cout << "--------------------------------------------------\n";

    Vehicle* curr = vehicleHead;
    int count = 0;
    while (curr != nullptr) {
        if (curr->organization == org) {
            cout << curr->vehicleId << "\t\t" << curr->type << "\t\t"
                << curr->capacity << "\t\t"
                << (curr->available ? "Available" : "In Use") << "\n";
            count++;
        }
        curr = curr->next;
    }

    if (count == 0) {
        cout << "No vehicles registered yet.\n";
    }
}

/* ==================== STORAGE MANAGEMENT ==================== */

void requestStorage() {
    int cropId;
    string organization;
    int quantity, budget;

    cout << "\n=== Request Cold Storage ===\n";
    cout << "\n=== My Crops ===\n";
    cout << "CropID\tType\tQuantity\tQuality\tPrice/kg\n";
    cout << "-----------------------------------------------\n";
    printFarmerCrops(cropBSTRoot, currentUserId);

    cout << "\nEnter Crop ID: "; cin >> cropId;

    Crop* crop = findCropById(cropBSTRoot, cropId);
    if (crop == nullptr || crop->farmerId != currentUserId) {
        cout << "Invalid crop ID or not your crop!\n";
        return;
    }

    cout << "Quantity (kg): "; cin >> quantity;

    if (quantity > crop->quantity) {
        cout << "Error: Requested quantity (" << quantity << " kg) exceeds available crop quantity ("
            << crop->quantity << " kg)!\n";
        return;
    }

    if (quantity <= 0) {
        cout << "Error: Quantity must be greater than 0!\n";
        return;
    }

    cout << "Budget (Rs): "; cin >> budget;

    cout << "\nAvailable Storage Centers:\n";
    StorageCenter* sc = storageCenterHead;
    while (sc != nullptr) {
        cout << sc->organization << " - Capacity: "
            << sc->availableCapacity << "/" << sc->totalCapacity
            << " kg, Temp: " << sc->temperature << "C\n";
        sc = sc->next;
    }

    cout << "Choose Organization: "; cin >> organization;

    StorageRequest* req = new StorageRequest;
    req->requestId = storageReqCount + 1;
    req->cropId = cropId;
    req->ownerId = currentUserId;
    req->cropName = crop->cropType;
    req->quantity = quantity;
    req->budget = budget;
    req->organization = organization;
    req->accepted = false;
    req->rejected = false;
    req->released = false;
    req->requesterId = currentUserId;
    req->next = nullptr;

    addStorageRequest(req);
    cout << "Storage request sent! Request ID: " << req->requestId << "\n";
    saveAllData();  // Save to JSON after storage request
}

void viewStorageRequests() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "storage_owner") {
        cout << "Only storage owners can view requests!\n";
        return;
    }

    string org = user->username; 

    cout << "\n=== Storage Requests for " << org << " ===\n";
    cout << "ReqID\tCrop\tQuantity\tBudget\tRequester\tStatus\n";
    cout << "-------------------------------------------------------------\n";

    StorageRequest* curr = storageHead;
    while (curr != nullptr) {
        if (curr->organization == org) {
            User* requester = findUserById(curr->requesterId);
            string requesterName = (requester != nullptr) ? requester->username : "Unknown";
            string status = curr->rejected ? "Rejected" : (curr->accepted ? "Accepted" : "Pending");
            cout << curr->requestId << "\t" << curr->cropName << "\t"
                << curr->quantity << "\t\t" << curr->budget << "\t"
                << requesterName << "\t" << status << "\n";
        }
        curr = curr->next;
    }
}

void rejectStorageRequest() {
    int reqId;
    cout << "\n=== Reject Storage Request ===\n";
    viewStorageRequests();
    cout << "Enter Request ID to reject: "; cin >> reqId;

    User* user = findUserById(currentUserId);
    if (user == nullptr) return;

    string org = user->username;

    StorageRequest* curr = storageHead;
    while (curr != nullptr) {
        if (curr->requestId == reqId && curr->organization == org && !curr->accepted && !curr->rejected) {
            curr->rejected = true;
            cout << "Request rejected successfully!\n";
            return;
        }
        curr = curr->next;
    }

    cout << "Request not found, already processed, or not for your organization!\n";
}

void registerStorageCenter() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "storage_owner") {
        cout << "Only storage owners can register storage centers!\n";
        return;
    }

    string org = user->username;

    StorageCenter* checkExisting = storageCenterHead;
    while (checkExisting != nullptr) {
        if (checkExisting->organization == org) {
            cout << "\nYou already have a storage center registered!\n";
            cout << "Organization: " << checkExisting->organization << "\n";
            cout << "Location: " << checkExisting->location << "\n";
            return;
        }
        checkExisting = checkExisting->next;
    }

    int capacity, pricePerKg;
    float temperature;
    string location;

    cout << "\n=== Register New Storage Center ===\n";
    cout << "Location (City): ";
    cin >> location;
    cout << "Total Capacity (kg): ";
    cin >> capacity;
    cout << "Temperature (°C): ";
    cin >> temperature;
    cout << "Price per kg (Rs): ";
    cin >> pricePerKg;

    StorageCenter* sc = new StorageCenter;
    sc->organization = org;
    sc->totalCapacity = capacity;
    sc->availableCapacity = capacity;
    sc->temperature = temperature;
    sc->location = location;
    sc->pricePerKg = pricePerKg;
    sc->registrationDate = readDate();
    sc->next = nullptr;

    addStorageCenter(sc);
    cout << "\nStorage center registered successfully!\n";
    cout << "Organization: " << org << "\n";
    cout << "Location: " << location << "\n";
    cout << "Price per kg: Rs. " << pricePerKg << "\n";
}

void allocateStorageOptimally() {
    User* user = findUserById(currentUserId);
    if (user == nullptr) {
        cout << "User not found!\n";
        return;
    }

    string org = user->username;

    StorageCenter* center = storageCenterHead;
    while (center != nullptr) {
        if (center->organization == org) break;
        center = center->next;
    }

    if (center == nullptr) {
        cout << "Storage center not found!\n";
        return;
    }

    int weights[100], values[100], n = 0;
    StorageRequest* requests[100];

    StorageRequest* curr = storageHead;
    while (curr != nullptr) {
        if (curr->organization == org && !curr->accepted && !curr->rejected) {
            weights[n] = curr->quantity;
            values[n] = curr->budget;
            requests[n] = curr;
            n++;
        }
        curr = curr->next;
    }

    if (n == 0) {
        cout << "No pending requests!\n";
        return;
    }

    bool selected[100] = { false };
    int totalBudget = knapsackStorageAllocation(center->availableCapacity, weights, values, n, selected);

    cout << "\n=== Optimal Storage Allocation ===\n";
    cout << "Total Budget Maximized: Rs. " << totalBudget << "\n\n";
    cout << "Accepted Requests:\n";

    int usedCapacity = 0;
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            requests[i]->accepted = true;
            requests[i]->approvalDate = getCurrentDate();
            usedCapacity += requests[i]->quantity;

            Crop* crop = findCropById(cropBSTRoot, requests[i]->cropId);
            if (crop != nullptr) {
                crop->storageQuantity = requests[i]->quantity;
                crop->storageCenter = org;
                crop->quantity -= requests[i]->quantity;
            }

            cout << "  - ReqID " << requests[i]->requestId << ": "
                << requests[i]->cropName << " (" << requests[i]->quantity
                << " kg, Rs. " << requests[i]->budget << ")\n";
        }
    }

    center->availableCapacity -= usedCapacity;
    cout << "\nRemaining Capacity: " << center->availableCapacity << " kg\n";
    cout << "All requesters have been notified.\n";
}

/* ==================== FARMER STORAGE MANAGEMENT ==================== */

void viewStoredCrops() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "farmer") {
        cout << "Only farmers can view stored crops!\n";
        return;
    }

    cout << "\n=== Your Stored Crops ===\n";
    cout << setw(8) << "CropID" << setw(12) << "Type" << setw(12) << "Qty Stored"
        << setw(12) << "Storage Ctr" << setw(15) << "Expiry Date\n";
    cout << "==================================================================================\n";

    Crop* node = cropBSTRoot;
    Crop* stack[100];
    int stackTop = 0;
    bool found = false;

//inorder traversal
    while (node != nullptr || stackTop > 0) {
        while (node != nullptr) {
            stack[stackTop++] = node;
            node = node->left;
        }
        node = stack[--stackTop];

        if (node->farmerId == currentUserId && node->storageQuantity > 0) {
            cout << setw(8) << node->cropId << setw(12) << node->cropType
                << setw(12) << node->storageQuantity << setw(12) << node->storageCenter
                << setw(15) << (to_string(node->expiryDate.day) + "/" + to_string(node->expiryDate.month)
                    + "/" + to_string(node->expiryDate.year)) << "\n";
            found = true;
        }
        node = node->right;
    }

    if (!found) {
        cout << "No crops in storage.\n";
    }
}

void releaseStoredCrop() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "farmer") {
        cout << "Only farmers can release stored crops!\n";
        return;
    }

    viewStoredCrops();

    int cropId, quantity;
    cout << "\nEnter Crop ID to release: "; cin >> cropId;
    cout << "Quantity to release (kg): "; cin >> quantity;

    Crop* crop = findCropById(cropBSTRoot, cropId);
    if (crop == nullptr || crop->farmerId != currentUserId || crop->storageQuantity == 0) {
        cout << "Invalid crop ID or not in storage!\n";
        return;
    }

    if (quantity > crop->storageQuantity) {
        cout << "Quantity exceeds stored amount!\n";
        return;
    }

    crop->storageQuantity -= quantity;
    crop->quantity += quantity;
    crop->storageCenter = "";

    StorageRequest* currReq = storageHead;
    while (currReq != nullptr) {
        if (currReq->cropId == cropId && currReq->requesterId == currentUserId && !currReq->released) {
            currReq->released = true;
            currReq->releaseDate = getCurrentDate();
            break;
        }
        currReq = currReq->next;
    }

    cout << "\n" << quantity << " kg of " << crop->cropType << " released to inventory!\n";
    cout << "Storage Quantity: " << crop->storageQuantity << " kg\n";
    cout << "Available Quantity: " << crop->quantity << " kg\n";
}

void updateStorageCapacity() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "storage_owner") {
        cout << "Only storage owners can update capacity!\n";
        return;
    }

    string org = user->username;
    int additionalCapacity;

    cout << "\n=== Update Storage Capacity ===\n";
    cout << "Additional Capacity (kg): "; cin >> additionalCapacity;

    StorageCenter* curr = storageCenterHead;
    while (curr != nullptr) {
        if (curr->organization == org) {
            curr->totalCapacity += additionalCapacity;
            curr->availableCapacity += additionalCapacity;
            cout << "\nCapacity updated successfully!\n";
            cout << "New Total Capacity: " << curr->totalCapacity << " kg\n";
            cout << "Available Capacity: " << curr->availableCapacity << " kg\n";
            return;
        }
        curr = curr->next;
    }

    cout << "Storage center not found!\n";
}

void viewMyStorageCenter() {
    User* user = findUserById(currentUserId);
    if (user == nullptr || user->role != "storage_owner") {
        cout << "Only storage owners can view their center!\n";
        return;
    }

    string org = user->username;

    cout << "\n=== My Storage Center ===\n";

    StorageCenter* curr = storageCenterHead;
    while (curr != nullptr) {
        if (curr->organization == org) {
            cout << "Organization: " << curr->organization << "\n";
            cout << "Location: " << curr->location << "\n";
            cout << "Registration Date: " << curr->registrationDate.day << "/"
                << curr->registrationDate.month << "/" << curr->registrationDate.year << "\n";
            cout << "Total Capacity: " << curr->totalCapacity << " kg\n";
            cout << "Available Capacity: " << curr->availableCapacity << " kg\n";
            cout << "Temperature: " << curr->temperature << "°C\n";
            cout << "Price per kg: Rs. " << curr->pricePerKg << "\n";
            cout << "Used Capacity: " << (curr->totalCapacity - curr->availableCapacity) << " kg\n";
            cout << "Utilization: " << ((curr->totalCapacity - curr->availableCapacity) * 100 / curr->totalCapacity) << "%\n";
            return;
        }
        curr = curr->next;
    }

    cout << "Storage center not found!\n";
}

/* ==================== ROUTE MANAGEMENT ==================== */

void findShortestRoute() {
    if (cityCount == 0) {
        cout << "No city data available!\n";
        return;
    }

    cout << "\n=== Shortest Route Finder ===\n";
    cout << "\nAvailable Cities:\n";
    for (int i = 0; i < cityCount; i++) {
        cout << i + 1 << ". " << cities[i].name << "\n";
    }

    int startChoice, endChoice;
    cout << "\nSelect Start City (1-" << cityCount << "): "; cin >> startChoice;
    cout << "Select Destination City (1-" << cityCount << "): "; cin >> endChoice;

    if (startChoice < 1 || startChoice > cityCount || endChoice < 1 || endChoice > cityCount) {
        cout << "Invalid city selection!\n";
        return;
    }

    int startIdx = startChoice - 1;
    int endIdx = endChoice - 1;

    if (startIdx == endIdx) {
        cout << "Start and destination are the same!\n";
        return;
    }

    int path[MAX_CITIES];
    int pathLength = 0;
    int distance = dijkstraShortestPath(startIdx, endIdx, path, pathLength);

    cout << "\n=== Route Information ===\n";
    cout << "Start: " << cities[startIdx].name << "\n";
    cout << "Destination: " << cities[endIdx].name << "\n";

    if (distance == 999999) {
        cout << "No route available between these cities!\n";
    }
    else {
        cout << "Shortest Distance: " << distance << " km\n";
        cout << "Estimated Time: " << (distance / 60.0) << " hours (at 60 km/h avg)\n";
        cout << "Path: ";
        for (int i = 0; i < pathLength; i++) {
            cout << cities[path[i]].name;
            if (i < pathLength - 1) cout << " -> ";
        }
        cout << "\n";
    }
}


void viewStatistics() {
    User* user = findUserById(currentUserId);
    if (user == nullptr) {
        cout << "User not found!\n";
        return;
    }

    string role = user->role;

    if (role == "farmer") {
        cout << "\n=== My Farmer Statistics ===\n\n";
        int myCrops = countCropsForFarmer(cropBSTRoot, currentUserId);
        cout << "CROP STATISTICS:\n";
        cout << "  My Total Crops Listed: " << myCrops << "\n\n";
        int myStorageReq = 0, acceptedStorage = 0;
        StorageRequest* sr = storageHead;
        while (sr != nullptr) {
            if (sr->requesterId == currentUserId) {
                myStorageReq++;
                if (sr->accepted) acceptedStorage++;
            }
            sr = sr->next;
        }
        cout << "STORAGE REQUESTS:\n";
        cout << "  Total Requests: " << myStorageReq << "\n";
        cout << "  Accepted: " << acceptedStorage << "\n";
        cout << "  Pending: " << (myStorageReq - acceptedStorage) << "\n";
    }
    else if (role == "buyer") {
        cout << "\n=== My Buyer Statistics ===\n\n";
        int myOrders = 0, paidOrders = 0, deliveredOrders = 0;
        Order* o = orderHead;
        while (o != nullptr) {
            if (o->buyerId == currentUserId) {
                myOrders++;
                if (o->paid) paidOrders++;
                if (o->delivered) deliveredOrders++;
            }
            o = o->next;
        }
        cout << "ORDER STATISTICS:\n";
        cout << "  My Total Orders: " << myOrders << "\n";
        cout << "  Paid Orders: " << paidOrders << "\n";
        cout << "  Delivered Orders: " << deliveredOrders << "\n";
        cout << "  Pending Payment: " << (myOrders - paidOrders) << "\n\n";
        int myTransportReq = 0, acceptedTransport = 0, completedTransport = 0;
        TransportRequest* tr = transportHead;
        while (tr != nullptr) {
            if (tr->requesterId == currentUserId) {
                myTransportReq++;
                if (tr->accepted) acceptedTransport++;
                if (tr->completed) completedTransport++;
            }
            tr = tr->next;
        }
        cout << "TRANSPORT REQUESTS:\n";
        cout << "  Total Requests: " << myTransportReq << "\n";
        cout << "  Accepted: " << acceptedTransport << "\n";
        cout << "  Completed: " << completedTransport << "\n";
        cout << "  Pending: " << (myTransportReq - acceptedTransport) << "\n";
    }
    else if (role == "transport_provider") {
        cout << "\n=== My Transport Provider Statistics ===\n\n";
        string org = user->username;
        int orgRequests = 0, acceptedReq = 0, completedReq = 0;
        TransportRequest* tr = transportHead;
        while (tr != nullptr) {
            if (tr->organization == org) {
                orgRequests++;
                if (tr->accepted) acceptedReq++;
                if (tr->completed) completedReq++;
            }
            tr = tr->next;
        }
        cout << "TRANSPORT STATISTICS for " << org << ":\n";
        cout << "  Total Requests Received: " << orgRequests << "\n";
        cout << "  Accepted: " << acceptedReq << "\n";
        cout << "  Completed: " << completedReq << "\n";
        cout << "  In Progress: " << (acceptedReq - completedReq) << "\n";
        cout << "  Pending: " << (orgRequests - acceptedReq) << "\n\n";
        int myVehicles = 0, availableVehicles = 0;
        Vehicle* v = vehicleHead;
        while (v != nullptr) {
            if (v->organization == org) {
                myVehicles++;
                if (v->available) availableVehicles++;
            }
            v = v->next;
        }
        cout << "VEHICLES for " << org << ":\n";
        cout << "  Total Vehicles: " << myVehicles << "\n";
        cout << "  Available: " << availableVehicles << "\n";
        cout << "  In Use: " << (myVehicles - availableVehicles) << "\n";
    }
    else if (role == "storage_owner") {
        cout << "\n=== My Storage Owner Statistics ===\n\n";
        string org = user->username;
        StorageCenter* center = storageCenterHead;
        while (center != nullptr) {
            if (center->organization == org) {
                cout << "STORAGE CENTER: " << org << "\n";
                cout << "  Location: " << center->location << "\n";
                cout << "  Total Capacity: " << center->totalCapacity << " kg\n";
                cout << "  Available Capacity: " << center->availableCapacity << " kg\n";
                cout << "  Used Capacity: " << (center->totalCapacity - center->availableCapacity) << " kg\n";
                if (center->totalCapacity > 0) {
                    cout << "  Utilization Rate: " << (100 - (center->availableCapacity * 100 / center->totalCapacity)) << "%\n";
                }
                cout << "  Temperature: " << center->temperature << "C\n\n";
                break;
            }
            center = center->next;
        }
        int orgRequests = 0, acceptedReq = 0;
        StorageRequest* sr = storageHead;
        while (sr != nullptr) {
            if (sr->organization == org) {
                orgRequests++;
                if (sr->accepted) acceptedReq++;
            }
            sr = sr->next;
        }
        cout << "STORAGE REQUESTS for " << org << ":\n";
        cout << "  Total Requests Received: " << orgRequests << "\n";
        cout << "  Accepted: " << acceptedReq << "\n";
        cout << "  Pending: " << (orgRequests - acceptedReq) << "\n";
    }
}

void viewSystemStatistics() {
    cout << "\n=== System Statistics & Analytics ===\n\n";
    int farmers = 0, buyers = 0, storageOwners = 0, transportProviders = 0;
    User* u = userListHead;
    while (u != nullptr) {
        if (u->role == "farmer") farmers++;
        else if (u->role == "buyer") buyers++;
        else if (u->role == "storage_owner") storageOwners++;
        else if (u->role == "transport_provider") transportProviders++;
        u = u->nextInList;
    }
    cout << "USER STATISTICS:\n";
    cout << "  Total Users: " << userCount << "\n";
    cout << "  Farmers: " << farmers << "\n";
    cout << "  Buyers: " << buyers << "\n";
    cout << "  Storage Owners: " << storageOwners << "\n";
    cout << "  Transport Providers: " << transportProviders << "\n\n";
    cout << "CROP STATISTICS:\n";
    cout << "  Total Crops Listed: " << cropCount << "\n";
    cout << "\n";
    int paidOrders = 0, deliveredOrders = 0;
    Order* o = orderHead;
    while (o != nullptr) {
        if (o->paid) paidOrders++;
        if (o->delivered) deliveredOrders++;
        o = o->next;
    }
    cout << "ORDER STATISTICS:\n";
    cout << "  Total Orders: " << orderCount << "\n";
    cout << "  Paid Orders: " << paidOrders << "\n";
    cout << "  Delivered Orders: " << deliveredOrders << "\n\n";
    int acceptedTransport = 0, completedTransport = 0;
    TransportRequest* tr = transportHead;
    while (tr != nullptr) {
        if (tr->accepted) acceptedTransport++;
        if (tr->completed) completedTransport++;
        tr = tr->next;
    }
    cout << "TRANSPORT STATISTICS:\n";
    cout << "  Total Requests: " << transportReqCount << "\n";
    cout << "  Accepted: " << acceptedTransport << "\n";
    cout << "  Completed: " << completedTransport << "\n";
    cout << "  Available Vehicles: " << vehicleCount << "\n\n";
    int totalCapacity = 0, availableCapacity = 0;
    StorageCenter* sc = storageCenterHead;
    while (sc != nullptr) {
        totalCapacity += sc->totalCapacity;
        availableCapacity += sc->availableCapacity;
        sc = sc->next;
    }
    int acceptedStorage = 0;
    StorageRequest* sr = storageHead;
    while (sr != nullptr) {
        if (sr->accepted) acceptedStorage++;
        sr = sr->next;
    }
    cout << "STORAGE STATISTICS:\n";
    cout << "  Total Centers: " << storageCenterCount << "\n";
    cout << "  Total Capacity: " << totalCapacity << " kg\n";
    cout << "  Available Capacity: " << availableCapacity << " kg\n";
    if (totalCapacity > 0) {
        cout << "  Utilization Rate: " << (100 - (availableCapacity * 100 / totalCapacity)) << "%\n";
    }
    cout << "  Storage Requests: " << storageReqCount << " (" << acceptedStorage << " accepted)\n";
}


void initializeSampleData() {
    // Sample Vehicles
    string vehicleOrgs[] = { "FastMove", "FastMove", "AgriTrans", "GreenWay", "AgriTrans" };
    for (int i = 0; i < 5; i++) {
        Vehicle* v = new Vehicle;
        v->vehicleId = i + 1;
        v->capacity = (i == 0) ? 500 : (i == 1) ? 1000 : (i == 2) ? 1500 : (i == 3) ? 2000 : 800;
        v->available = true;
        v->type = (i == 0) ? "Van" : (i == 1) ? "Truck" : (i == 2) ? "Large Truck" : (i == 3) ? "Container" : "Pickup";
        v->organization = vehicleOrgs[i];
        v->next = nullptr;
        addVehicle(v);
    }

    // Sample Storage Centers
    string orgs[] = { "ColdHub", "AgriStore", "FarmSafe", "FreshKeep" };
    int caps[] = { 10000, 8000, 6000, 12000 };
    float temps[] = { 4.0, 5.0, 3.0, 4.5 };
    string locs[] = { "Lahore", "Karachi", "Islamabad", "Multan" };
    int prices[] = { 50, 45, 55, 48 };  // Price per kg

    for (int i = 0; i < 4; i++) {
        StorageCenter* sc = new StorageCenter;
        sc->organization = orgs[i];
        sc->totalCapacity = caps[i];
        sc->availableCapacity = caps[i];
        sc->temperature = temps[i];
        sc->location = locs[i];
        sc->pricePerKg = prices[i];
        sc->registrationDate = { 15, 12, 2025 };
        sc->next = nullptr;
        addStorageCenter(sc);
    }

    // Sample Users
    // Farmer
    User* u1 = new User;
    u1->userId = 1001;
    u1->username = "farmer1";
    u1->password = "pass123";
    u1->role = "farmer";
    u1->active = true;
    u1->nextInBucket = nullptr;
    u1->nextInList = nullptr;
    insertUser(u1);

    // Buyer
    User* u2 = new User;
    u2->userId = 1002;
    u2->username = "buyer1";
    u2->password = "pass123";
    u2->role = "buyer";
    u2->active = true;
    u2->nextInBucket = nullptr;
    u2->nextInList = nullptr;
    insertUser(u2);

    // Transport Providers
    string transportOrgs[] = { "FastMove", "AgriTrans", "GreenWay" };
    for (int i = 0; i < 3; i++) {
        User* u = new User;
        u->userId = 1100 + i;
        u->username = transportOrgs[i];
        u->password = "pass123";
        u->role = "transport_provider";
        u->active = true;
        u->nextInBucket = nullptr;
        u->nextInList = nullptr;
        insertUser(u);
    }

    // Storage Owners
    for (int i = 0; i < 4; i++) {
        User* u = new User;
        u->userId = 1200 + i;
        u->username = orgs[i];
        u->password = "pass123";
        u->role = "storage_owner";
        u->active = true;
        u->nextInBucket = nullptr;
        u->nextInList = nullptr;
        insertUser(u);
    }

    // Sample Crops
    string cropTypes[] = { "Wheat", "Rice", "Mango" };
    int cropPrices[] = { 60, 80, 150 };
    int quantities[] = { 5000, 3000, 2000 };
    string qualities[] = { "A", "B", "A" };

    for (int i = 0; i < 3; i++) {
        Crop* c = new Crop;
        c->cropId = 2001 + i;
        c->farmerId = 1001;
        c->cropType = cropTypes[i];
        c->quantity = quantities[i];
        c->storageQuantity = 0;
        c->quality = qualities[i];
        c->pricePerKg = cropPrices[i];
        c->available = true;
        c->dateAdded = { 1, 1, 2026 };
        c->expiryDate = { 31, 12, 2026 };
        c->storageCenter = "";
        c->left = c->right = nullptr;
        cropBSTRoot = insertCropBST(cropBSTRoot, c);
        cropCount++;
    }

    // Sample Cities Network
    string cityNames[] = { "Lahore", "Islamabad", "Karachi", "Multan", "Faisalabad", "Peshawar", "Quetta", "Sialkot" };
    for (int i = 0; i < 8; i++) {
        cities[i].name = cityNames[i];
        cities[i].adjList = nullptr;
    }
    cityCount = 8;

    addEdge(0, 1, 380); addEdge(1, 0, 380);  // Lahore-Islamabad
    addEdge(0, 3, 340); addEdge(3, 0, 340);  // Lahore-Multan
    addEdge(0, 4, 135); addEdge(4, 0, 135);  // Lahore-Faisalabad
    addEdge(1, 5, 170); addEdge(5, 1, 170);  // Islamabad-Peshawar
    addEdge(1, 7, 115); addEdge(7, 1, 115);  // Islamabad-Sialkot
    addEdge(2, 3, 880); addEdge(3, 2, 880);  // Karachi-Multan
    addEdge(2, 6, 680); addEdge(6, 2, 680);  // Karachi-Quetta
    addEdge(3, 4, 210); addEdge(4, 3, 210);  // Multan-Faisalabad
    addEdge(5, 6, 1400); addEdge(6, 5, 1400); // Peshawar-Quetta
    addEdge(0, 7, 115); addEdge(7, 0, 115);  // Lahore-Sialkot

    cout << "Sample data initialized successfully!\n";
    cout << "Test Logins:\n";
    cout << "  Farmer: farmer1 / pass123\n";
    cout << "  Buyer: buyer1 / pass123\n";
    cout << "  Transport: FastMove, AgriTrans, GreenWay / pass123\n";
    cout << "  Storage: ColdHub, AgriStore, FarmSafe, FreshKeep / pass123\n";
}

//menus of users

void farmerMenu() {
    int choice;
    do {
        cout << "\n===== FARMER MENU =====\n";
        cout << "1. Add New Crop\n";
        cout << "2. View My Crops\n";
        cout << "3. Request Cold Storage\n";
        cout << "4. View/Respond to Crop Requests\n";
        cout << "5. Request Transport Service\n";
        cout << "6. Track My Requests (Storage & Transport)\n";
        cout << "7. View My Statistics\n";
        cout << "8. Check My Notifications\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addCrop(); break;
        case 2: {
            int subChoice;
            cout << "\n=== View My Crops ===\n";
            cout << "1. View All Crops\n";
            cout << "2. View Stored Crops\n";
            cout << "3. Release Stored Crop\n";
            cout << "0. Back\n";
            cout << "Choice: ";
            cin >> subChoice;

            if (subChoice == 1) {
                cout << "\n=== My Crops ===\n";
                cout << "CropID\tType\tQuantity\tQuality\tPrice/kg\tDate\n";
                cout << "---------------------------------------------------------\n";
                printFarmerCrops(cropBSTRoot, currentUserId);
            }
            else if (subChoice == 2) {
                viewStoredCrops();
                int doRelease;
                cout << "\nRelease a stored crop? (1=Yes, 0=No): ";
                cin >> doRelease;
                if (doRelease == 1) {
                    releaseStoredCrop();
                }
            }
            else if (subChoice == 3) {
                releaseStoredCrop();
            }
            break;
        }
        case 3: requestStorage(); break;
        case 4: {
            viewCropRequests();
            int respond;
            cout << "\nRespond to a request now? (1=Yes, 0=No): ";
            cin >> respond;
            if (respond == 1) {
                approveCropRequest();
            }
            break;
        }
        case 5: requestTransport(); break;
        case 6: {
            cout << "\n=== My Storage Requests ===\n";
            cout << "ReqID\tCrop\tQuantity\tOrganization\tStatus\n";
            cout << "---------------------------------------------------\n";
            StorageRequest* sr = storageHead;
            while (sr != nullptr) {
                if (sr->requesterId == currentUserId) {
                    string status = sr->rejected ? "Rejected" : (sr->accepted ? "Accepted" : "Pending");
                    cout << sr->requestId << "\t" << sr->cropName << "\t"
                        << sr->quantity << "\t\t" << sr->organization << "\t\t"
                        << status << "\n";
                }
                sr = sr->next;
            }
            cout << "\n=== My Transport Requests ===\n";
            cout << "ReqID\tCropCount\tWeight\tOrganization\tStatus\n";
            cout << "-----------------------------------------------------\n";
            TransportRequest* tr = transportHead;
            while (tr != nullptr) {
                if (tr->requesterId == currentUserId) {
                    string status;
                    if (tr->rejected) status = "Rejected";
                    else if (tr->completed) status = "Completed";
                    else if (tr->accepted) status = "In Progress";
                    else status = "Pending";
                    cout << tr->requestId << "\t" << tr->cropCount << "\t\t"
                        << tr->weight << "\t" << tr->organization << "\t\t"
                        << status << "\n";
                }
                tr = tr->next;
            }
            break;
        }
        case 7: viewStatistics(); break;
        case 8: {
            cout << "\n=== My Notifications ===\n";
            bool found = false;
            StorageRequest* sr = storageHead;
            while (sr != nullptr) {
                if (sr->requesterId == currentUserId && sr->accepted) {
                    cout << "✓ Storage request #" << sr->requestId << " accepted by " << sr->organization << "\n";
                    found = true;
                }
                sr = sr->next;
            }
            TransportRequest* tr = transportHead;
            while (tr != nullptr) {
                if (tr->requesterId == currentUserId) {
                    if (tr->accepted) {
                        cout << "✓ Transport request #" << tr->requestId << " accepted by " << tr->organization;
                        if (tr->completed) cout << " and completed";
                        cout << "\n";
                        found = true;
                    }
                    else if (tr->rejected) {
                        cout << "✗ Transport request #" << tr->requestId << " rejected by " << tr->organization
                            << " (insufficient vehicle capacity)\n";
                        found = true;
                    }
                }
                tr = tr->next;
            }
            if (!found) cout << "No new notifications.\n";
            break;
        }
        }
    } while (choice != 0);
}

void buyerMenu() {
    int choice;
    do {
        cout << "\n===== BUYER MENU =====\n";
        cout << "1. View All Crops\n";
        cout << "2. Compare Crop Prices\n";
        cout << "3. Request Crop from Farmer\n";
        cout << "4. View My Crop Requests\n";
        cout << "5. Pay for Approved Order\n";
        cout << "6. Request Transport Service\n";
        cout << "7. Track My Deliveries\n";
        cout << "8. View My Statistics\n";
        cout << "9. Check My Notifications\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: viewAllCrops(); break;
        case 2: compareCropPrices(); break;
        case 3: requestCrop(); break;
        case 4: viewMyOrders(); break;
        case 5: payForOrder(); break;
        case 6: requestTransport(); break;
        case 7: {
            cout << "\n=== My Orders & Delivery Status ===\n";
            cout << "OrderID\tCropID\tQuantity\tApproved\tPaid\tDelivered\tDate\n";
            cout << "------------------------------------------------------\n";
            Order* o = orderHead;
            while (o != nullptr) {
                if (o->buyerId == currentUserId) {
                    cout << o->orderId << "\t" << o->cropId << "\t"
                        << o->quantity << "\t\t"
                        << (o->farmerApproved ? "Yes" : "No") << "\t"
                        << (o->paid ? "Yes" : "No") << "\t"
                        << (o->delivered ? "Yes" : "No") << "\t"
                        << formatDate(o->orderDate) << "\n";
                }
                o = o->next;
            }
            cout << "\nTRANSPORT REQUESTS:\n";
            cout << "ReqID\tCropCount\tWeight\tOrganization\tStatus\n";
            cout << "-----------------------------------------------------\n";
            TransportRequest* tr = transportHead;
            while (tr != nullptr) {
                if (tr->requesterId == currentUserId) {
                    string status;
                    if (tr->rejected) status = "Rejected";
                    else if (tr->completed) status = "Completed";
                    else if (tr->accepted) status = "In Progress";
                    else status = "Pending";
                    cout << tr->requestId << "\t" << tr->cropCount << "\t\t"
                        << tr->weight << "\t" << tr->organization << "\t\t"
                        << status << "\n";
                }
                tr = tr->next;
            }
            break;
        }
        case 8: viewStatistics(); break;
        case 9: {
            cout << "\n=== My Notifications ===\n";
            bool found = false;
            TransportRequest* tr = transportHead;
            while (tr != nullptr) {
                if (tr->requesterId == currentUserId && tr->accepted) {
                    cout << "✓ Transport request #" << tr->requestId << " accepted by " << tr->organization;
                    if (tr->completed) cout << " and completed";
                    cout << "\n";
                    found = true;
                }
                tr = tr->next;
            }
            if (!found) cout << "No new notifications.\n";
            break;
        }
        }
    } while (choice != 0);
}

void transportProviderMenu() {
    int choice;
    do {
        cout << "\n===== TRANSPORT PROVIDER MENU =====\n";
        cout << "1. Register New Vehicle\n";
        cout << "2. View My Vehicles\n";
        cout << "3. View & Respond to Transport Requests\n";
        cout << "4. Complete Delivery\n";
        cout << "5. View My Statistics\n";
        cout << "6. Find Optimal Route\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: registerVehicle(); break;
        case 2: viewMyVehicles(); break;
        case 3: {
            cout << "\n=== View & Respond to Transport Requests ===\n";
            viewTransportRequests();
            int reqId;
            cout << "\nEnter Request ID to respond to (0 to skip): "; cin >> reqId;
            if (reqId != 0) {
                int response;
                cout << "1. Accept  |  2. Reject: "; cin >> response;
                if (response == 1) {
                    cout << "\n=== Accept Request ===\n";
                    TransportRequest* curr = transportHead;
                    while (curr != nullptr) {
                        if (curr->requestId == reqId && !curr->accepted) {
                            User* user = findUserById(currentUserId);
                            string org = (user != nullptr) ? user->username : "";


                            int remainingWeight = curr->weight;
                            int vehiclesAssigned = 0;


                            Vehicle* v = vehicleHead;
                            while (v != nullptr && remainingWeight > 0) {
                                if (v->organization == org && v->available) {
                                    if (v->capacity >= remainingWeight) {

                                        v->available = false;
                                        vehiclesAssigned++;
                                        cout << "Vehicle " << v->vehicleId << " assigned (Capacity: "
                                            << v->capacity << " kg, carrying " << remainingWeight << " kg)\n";
                                        remainingWeight = 0;
                                    }
                                    else {

                                        v->available = false;
                                        vehiclesAssigned++;
                                        cout << "Vehicle " << v->vehicleId << " assigned (Capacity: "
                                            << v->capacity << " kg, fully loaded)\n";
                                        remainingWeight -= v->capacity;
                                    }
                                }
                                v = v->next;
                            }

                            if (remainingWeight > 0) {
                                curr->rejected = true;

                                Vehicle* restoreV = vehicleHead;
                                int restored = 0;
                                while (restoreV != nullptr && restored < vehiclesAssigned) {
                                    if (restoreV->organization == org && !restoreV->available) {
                                        restoreV->available = true;
                                        restored++;
                                    }
                                    restoreV = restoreV->next;
                                }

                                cout << "Request rejected: Insufficient vehicle capacity! Need " << remainingWeight
                                    << " kg more capacity.\n";
                                cout << "Farmer will be notified of rejection.\n";
                            }
                            else {
                                curr->accepted = true;
                                cout << "\nRequest accepted! Total " << vehiclesAssigned
                                    << " vehicle(s) assigned for " << curr->weight << " kg\n";
                            }
                            break;
                        }
                        curr = curr->next;
                    }
                    cout << "Request not found or already accepted!\n";
                }
                else if (response == 2) {
                    cout << "\n=== Reject Request ===\n";
                    TransportRequest* curr = transportHead;
                    while (curr != nullptr) {
                        if (curr->requestId == reqId && !curr->rejected && !curr->accepted) {
                            curr->rejected = true;
                            cout << "Request rejected!\n";
                            break;
                        }
                        curr = curr->next;
                    }
                    cout << "Request not found or already processed!\n";
                }
            }
            break;
        }
        case 4: completeDelivery(); break;
        case 5: viewStatistics(); break;
        case 6: findShortestRoute(); break;
        }
    } while (choice != 0);
}

void storageOwnerMenu() {
    int choice;
    do {
        cout << "\n===== STORAGE OWNER MENU =====\n";
        cout << "1. View My Storage Center\n";
        cout << "2. Update Storage Capacity\n";
        cout << "3. View & Respond to Storage Requests\n";
        cout << "4. Allocate Storage Optimally\n";
        cout << "5. View My Statistics\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: viewMyStorageCenter(); break;
        case 2: updateStorageCapacity(); break;
        case 3: {
            viewStorageRequests();
            int reqId;
            cout << "\nEnter Request ID to respond to (0 to skip): ";
            cin >> reqId;
            if (reqId != 0) {
                int action;
                cout << "1. Accept  |  2. Reject: ";
                cin >> action;

                User* user = findUserById(currentUserId);
                string org = (user != nullptr) ? user->username : "";
                StorageCenter* center = storageCenterHead;
                while (center != nullptr && center->organization != org) {
                    center = center->next;
                }

                StorageRequest* curr = storageHead;
                bool requestProcessed = false;
                while (curr != nullptr) {
                    if (curr->requestId == reqId && curr->organization == org && !curr->accepted && !curr->rejected) {
                        if (action == 1) {
                            if (center == nullptr) {
                                cout << "No storage center found!\n";
                                break;
                            }
                            if (curr->quantity > center->availableCapacity) {
                                cout << "Not enough capacity available!\n";
                                break;
                            }
                            curr->accepted = true;
                            curr->approvalDate = getCurrentDate();
                            center->availableCapacity -= curr->quantity;

                            Crop* crop = findCropById(cropBSTRoot, curr->cropId);
                            if (crop != nullptr) {
                                crop->storageQuantity = curr->quantity;
                                crop->storageCenter = org;
                                crop->quantity -= curr->quantity;
                            }

                            cout << "Request accepted and capacity updated!\n";
                            requestProcessed = true;
                        }
                        else if (action == 2) {
                            curr->rejected = true;
                            cout << "Request rejected successfully!\n";
                            requestProcessed = true;
                        }
                        break;
                    }
                    curr = curr->next;
                }
                if (!requestProcessed) {
                    cout << "Request not found or already processed!\n";
                }
            }
            break;
        }
        case 4: allocateStorageOptimally(); break;
        case 5: viewStatistics(); break;
        }
    } while (choice != 0);
}

// Include JSON handler implementation
#include "json_handler.cpp"

int main() {
    // Load existing data from JSON files
    loadAllData();
    
    // If no data loaded, initialize sample data
    if (userCount == 0) {
        cout << "\n[First Run] Initializing sample data...\n";
        initializeSampleData();
    }

    int choice;
    do {
        cout << "\n===============================================\n";
        cout << "||       AGRICONNECT PAKISTAN PLATFORM       ||\n";
        cout << "||        Connecting Farmers, Buyers,        ||\n";
        cout << "||  Transport Providers, and Storage Owners  ||\n";
        cout << "===============================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View All Users\n";
        cout << "4. View Statistics\n";
        cout << "5. Find Shortest Route\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: registerUser(); break;
        case 2: {
            if (loginUser()) {
                User* user = findUserById(currentUserId);
                if (user != nullptr) {
                    string role = user->role;

                    if (role == "farmer") farmerMenu();
                    else if (role == "buyer") buyerMenu();
                    else if (role == "transport_provider") transportProviderMenu();
                    else if (role == "storage_owner") storageOwnerMenu();

                    currentUserId = -1;
                }
            }
            break;
        }
        case 3: viewAllUsers(); break;
        case 4: viewSystemStatistics(); break;
        case 5: findShortestRoute(); break;
        }
    } while (choice != 0);

    cout << "\n[Saving] Saving all data to JSON files...\n";
    saveAllData();
    cout << "Thank you for using AgriConnect Pakistan!\n";
    return 0;
}