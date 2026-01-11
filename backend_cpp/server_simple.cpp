#include <iostream>
#include <winsock2.h>
#include <thread>
#include <string>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 8080;
const int BUFFER_SIZE = 2048;

string handleRequest(string path, string method, string body) {
    if (method == "OPTIONS") {
        return "{\"status\": \"ok\"}";
    }
    if (method == "POST" && path == "/login") {
        // Check if user exists in body - for now accept any login
        if (body.find("username") != string::npos && body.find("password") != string::npos) {
            return "{\"success\": true, \"message\": \"Login successful\", \"user\": {\"id\": 1, \"username\": \"testuser\", \"role\": \"farmer\"}}";
        }
        return "{\"success\": false, \"message\": \"Invalid credentials. Please register first!\"}";
    }
    if (method == "POST" && path == "/register") {
        return "{\"success\": true, \"message\": \"Registration successful\", \"user\": {\"id\": 1, \"username\": \"newuser\"}}";
    }
    if (method == "GET" && path == "/status") {
        return "{\"status\": \"ok\", \"message\": \"Server running\"}";
    }
    if (method == "GET" && path == "/users") {
        return "{\"users\": [{\"id\": 1, \"username\": \"farmer1\", \"role\": \"farmer\"}]}";
    }
    if (method == "GET" && path == "/crops") {
        return "{\"crops\": [{\"id\": 1, \"type\": \"Wheat\", \"quantity\": 100, \"price\": 50}]}";
    }
    return "{\"status\": \"ok\", \"message\": \"AgriConnect Backend Server Running on Port 8080\"}";
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
        
        cout << "[" << method << " " << path << "]\n";
        
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
    
    // Allow address reuse
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
    cout << "  http://localhost:8080\n";
    cout << "  Status: RUNNING\n";
    cout << "========================================\n";
    
    SOCKET clientSocket;
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    
    while (true) {
        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (clientSocket != INVALID_SOCKET) {
            cout << "Request received\n";
            thread t(handleClient, clientSocket);
            t.detach();
        }
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
