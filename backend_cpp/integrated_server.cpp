#include <iostream>
#include <winsock2.h>
#include <thread>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 4096;
const int HASH_TABLE_SIZE = 101;

// Data structures from agriconnect_simple.cpp
struct SimpleDate {
    int day;
    int month;
    int year;
};

struct User {
    int userId;
    string username;
    string password;
    string role;
    string email;
    string phone;
    bool active;
    User* nextInBucket;
    User* nextInList;
};

struct Crop {
    int cropId;
    int farmerId;
    string cropType;
    int quantity;
    string quality;
    int pricePerKg;
    bool available;
    SimpleDate dateAdded;
    Crop* left;
    Crop* right;
};

// Global data storage
User* userHashTable[HASH_TABLE_SIZE] = { nullptr };
User* userListHead = nullptr;
Crop* cropBSTRoot = nullptr;
int userCount = 0;
int cropCount = 0;

// Utility functions
int hashUsername(string username) {
    int h = 0;
    for (size_t i = 0; i < username.length(); i++)
        h = (h * 31 + username[i]) % HASH_TABLE_SIZE;
    return h;
}

int hashID(string key) {
    int h = 0;
    for (size_t i = 0; i < key.length(); i++)
        h = (h * 31 + key[i]) % 100000;
    return h;
}

SimpleDate getCurrentDate() { return { 12, 1, 2026 }; }

string formatDate(SimpleDate d) {
    ostringstream oss;
    oss << setfill('0') << setw(2) << d.day << "-" << setw(2) << d.month << "-" << d.year;
    return oss.str();
}

// User management
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

// Crop BST management
Crop* insertCropBST(Crop* root, Crop* newCrop) {
    if (root == nullptr) return newCrop;
    if (newCrop->pricePerKg < root->pricePerKg)
        root->left = insertCropBST(root->left, newCrop);
    else
        root->right = insertCropBST(root->right, newCrop);
    return root;
}

void getCropsJSON(Crop* root, stringstream& ss, bool& first) {
    if (root == nullptr) return;
    getCropsJSON(root->left, ss, first);
    if (root->available) {
        if (!first) ss << ",";
        first = false;
        User* farmer = findUserById(root->farmerId);
        string farmerName = (farmer != nullptr) ? farmer->username : "Unknown";
        ss << "{\"id\":" << root->cropId 
           << ",\"type\":\"" << root->cropType << "\""
           << ",\"quantity\":" << root->quantity
           << ",\"quality\":\"" << root->quality << "\""
           << ",\"price\":" << root->pricePerKg
           << ",\"farmer\":\"" << farmerName << "\""
           << ",\"date\":\"" << formatDate(root->dateAdded) << "\"}";
    }
    getCropsJSON(root->right, ss, first);
}

// JSON parsing helper
string extractJsonValue(const string& json, const string& key) {
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == string::npos) return "";
    
    size_t colonPos = json.find(":", keyPos);
    if (colonPos == string::npos) return "";
    
    size_t valueStart = json.find("\"", colonPos);
    if (valueStart == string::npos) return "";
    
    size_t valueEnd = json.find("\"", valueStart + 1);
    if (valueEnd == string::npos) return "";
    
    return json.substr(valueStart + 1, valueEnd - valueStart - 1);
}

// HTTP Request handler
string handleRequest(string path, string method, string body) {
    if (method == "OPTIONS") {
        return "{\"status\": \"ok\"}";
    }
    
    // Registration endpoint
    if (method == "POST" && path == "/register") {
        string username = extractJsonValue(body, "username");
        string password = extractJsonValue(body, "password");
        string role = extractJsonValue(body, "role");
        string email = extractJsonValue(body, "email");
        string phone = extractJsonValue(body, "phone");
        
        if (username.empty() || password.empty()) {
            return "{\"success\": false, \"message\": \"Username and password are required\"}";
        }
        
        if (findUserByUsername(username) != nullptr) {
            return "{\"success\": false, \"message\": \"Username already exists. Please choose another.\"}";
        }
        
        User* newUser = new User;
        newUser->userId = hashID(username + to_string(userCount));
        newUser->username = username;
        newUser->password = password;
        newUser->role = role.empty() ? "farmer" : role;
        newUser->email = email;
        newUser->phone = phone;
        newUser->active = true;
        newUser->nextInBucket = nullptr;
        newUser->nextInList = nullptr;
        
        insertUser(newUser);
        
        cout << "[REGISTER] New user: " << username << " (" << newUser->role << ")\n";
        
        return "{\"success\": true, \"message\": \"Registration successful! You can now login.\", \"user\": {\"id\": " + 
               to_string(newUser->userId) + ", \"username\": \"" + newUser->username + "\", \"role\": \"" + newUser->role + "\"}}";
    }
    
    // Login endpoint
    if (method == "POST" && path == "/login") {
        string username = extractJsonValue(body, "username");
        string password = extractJsonValue(body, "password");
        
        if (username.empty() || password.empty()) {
            return "{\"success\": false, \"message\": \"Username and password are required\"}";
        }
        
        User* user = findUserByUsername(username);
        if (user == nullptr) {
            return "{\"success\": false, \"message\": \"User not found. Please register first!\"}";
        }
        
        if (user->password != password) {
            return "{\"success\": false, \"message\": \"Invalid password. Please try again.\"}";
        }
        
        cout << "[LOGIN] User: " << username << " (" << user->role << ")\n";
        
        return "{\"success\": true, \"message\": \"Login successful!\", \"user\": {\"id\": " + 
               to_string(user->userId) + ", \"username\": \"" + user->username + 
               "\", \"role\": \"" + user->role + "\", \"email\": \"" + user->email + "\"}}";
    }
    
    // Status endpoint
    if (method == "GET" && path == "/status") {
        return "{\"status\": \"ok\", \"message\": \"AgriConnect Server Running\", \"users\": " + 
               to_string(userCount) + ", \"crops\": " + to_string(cropCount) + "}";
    }
    
    // Get all users
    if (method == "GET" && path == "/users") {
        stringstream ss;
        ss << "{\"users\":[";
        User* curr = userListHead;
        bool first = true;
        while (curr != nullptr) {
            if (!first) ss << ",";
            first = false;
            ss << "{\"id\":" << curr->userId 
               << ",\"username\":\"" << curr->username << "\""
               << ",\"role\":\"" << curr->role << "\"}";
            curr = curr->nextInList;
        }
        ss << "]}";
        return ss.str();
    }
    
    // Get all crops
    if (method == "GET" && path == "/crops") {
        stringstream ss;
        ss << "{\"crops\":[";
        bool first = true;
        getCropsJSON(cropBSTRoot, ss, first);
        ss << "]}";
        return ss.str();
    }
    
    return "{\"status\": \"ok\", \"message\": \"AgriConnect Backend Server\"}";
}

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytesReceived > 0) {
        string request(buffer);
        
        // Parse request line
        string method, path;
        size_t pos = request.find(' ');
        if (pos != string::npos) {
            method = request.substr(0, pos);
            size_t pos2 = request.find(' ', pos + 1);
            if (pos2 != string::npos) {
                path = request.substr(pos + 1, pos2 - pos - 1);
            }
        }
        
        // Handle OPTIONS for CORS
        if (method == "OPTIONS") {
            string response = "HTTP/1.1 200 OK\r\n"
                             "Access-Control-Allow-Origin: *\r\n"
                             "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                             "Access-Control-Allow-Headers: Content-Type\r\n"
                             "Connection: close\r\n"
                             "Content-Length: 0\r\n\r\n";
            send(clientSocket, response.c_str(), response.length(), 0);
            closesocket(clientSocket);
            return;
        }
        
        // Get body if POST
        string body;
        size_t bodyStart = request.find("\r\n\r\n");
        if (bodyStart != string::npos) {
            body = request.substr(bodyStart + 4);
        }
        
        // Get response
        string jsonBody = handleRequest(path, method, body);
        
        // Build HTTP response
        string response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: application/json\r\n"
                         "Access-Control-Allow-Origin: *\r\n"
                         "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                         "Access-Control-Allow-Headers: Content-Type\r\n"
                         "Connection: close\r\n"
                         "Content-Length: " + to_string(jsonBody.length()) + "\r\n"
                         "\r\n" + jsonBody;
        
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
        return 1;
    }
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "socket failed\n";
        WSACleanup();
        return 1;
    }
    
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
    
    int reuse = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
    
    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "========================================\n";
    cout << "  AgriConnect Backend Server\n";
    cout << "  http://localhost:" << PORT << "\n";
    cout << "  Status: RUNNING\n";
    cout << "  Data Structures: Hash Table + BST\n";
    cout << "========================================\n\n";
    
    SOCKET clientSocket;
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    
    while (true) {
        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (clientSocket != INVALID_SOCKET) {
            thread t(handleClient, clientSocket);
            t.detach();
        }
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
