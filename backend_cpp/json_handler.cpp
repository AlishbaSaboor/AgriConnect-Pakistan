#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include "json_handler.h"

using namespace std;
namespace fs = filesystem;

// Create data folder if it doesn't exist
void ensureDataFolder() {
    try {
        if (!fs::exists("data")) {
            fs::create_directory("data");
            cout << "[JSON] Data folder created.\n";
        }
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to create data folder: " << e.what() << "\n";
    }
}

// Helper: Escape special characters for JSON
string escapeJSON(const string& str) {
    string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (c < 0x20) result += "\\u" + to_string(int(c));
                else result += c;
        }
    }
    return result;
}

// Helper: Unescape JSON strings
string unescapeJSON(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case '"': result += '"'; i++; break;
                case '\\': result += '\\'; i++; break;
                case 'b': result += '\b'; i++; break;
                case 'f': result += '\f'; i++; break;
                case 'n': result += '\n'; i++; break;
                case 'r': result += '\r'; i++; break;
                case 't': result += '\t'; i++; break;
                default: result += str[i];
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

// Helper: Extract value from JSON string
string extractJSONValue(const string& json, const string& key, bool isString = true) {
    string searchKey = "\"" + key + "\":";
    size_t keyPos = json.find(searchKey);
    if (keyPos == string::npos) return "";
    
    size_t startPos = keyPos + searchKey.length();
    while (startPos < json.length() && (json[startPos] == ' ' || json[startPos] == '\n' || json[startPos] == '\t'))
        startPos++;
    
    if (isString) {
        if (json[startPos] != '"') return "";
        startPos++;
        size_t endPos = startPos;
        while (endPos < json.length() && json[endPos] != '"') {
            if (json[endPos] == '\\') endPos++;
            endPos++;
        }
        return unescapeJSON(json.substr(startPos, endPos - startPos));
    } else {
        size_t endPos = startPos;
        while (endPos < json.length() && json[endPos] != ',' && json[endPos] != '}' && json[endPos] != ']')
            endPos++;
        return json.substr(startPos, endPos - startPos);
    }
}

/* ==================== USERS ==================== */

void saveUsersToJSON() {
    try {
        ofstream file("data/users.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/users.json for writing.\n";
            return;
        }

        file << "[\n";
        User* curr = userListHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"userId\": " << curr->userId << ",\n";
            file << "    \"username\": \"" << escapeJSON(curr->username) << "\",\n";
            file << "    \"password\": \"" << escapeJSON(curr->password) << "\",\n";
            file << "    \"role\": \"" << escapeJSON(curr->role) << "\",\n";
            file << "    \"active\": " << (curr->active ? "true" : "false") << "\n";
            file << "  }";

            curr = curr->nextInList;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " users to data/users.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save users: " << e.what() << "\n";
    }
}

void loadUsersFromJSON() {
    try {
        ifstream file("data/users.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing users.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"userId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            User* newUser = new User;
            newUser->userId = stoi(extractJSONValue(obj, "userId", false));
            newUser->username = extractJSONValue(obj, "username", true);
            newUser->password = extractJSONValue(obj, "password", true);
            newUser->role = extractJSONValue(obj, "role", true);
            newUser->active = extractJSONValue(obj, "active", false) == "true";
            newUser->nextInBucket = nullptr;
            newUser->nextInList = nullptr;

            insertUser(newUser);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " users from data/users.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load users: " << e.what() << "\n";
    }
}

/* ==================== CROPS ==================== */

void saveCropsToJSON() {
    try {
        ofstream file("data/crops.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/crops.json for writing.\n";
            return;
        }

        file << "[\n";
        
        // In-order traversal of BST
        function<void(Crop*, bool&)> traverse = [&](Crop* node, bool& first) {
            if (node == nullptr) return;
            traverse(node->left, first);

            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"cropId\": " << node->cropId << ",\n";
            file << "    \"farmerId\": " << node->farmerId << ",\n";
            file << "    \"cropType\": \"" << escapeJSON(node->cropType) << "\",\n";
            file << "    \"quantity\": " << node->quantity << ",\n";
            file << "    \"storageQuantity\": " << node->storageQuantity << ",\n";
            file << "    \"quality\": \"" << escapeJSON(node->quality) << "\",\n";
            file << "    \"pricePerKg\": " << node->pricePerKg << ",\n";
            file << "    \"available\": " << (node->available ? "true" : "false") << ",\n";
            file << "    \"dateAdded\": \"" << formatDate(node->dateAdded) << "\",\n";
            file << "    \"expiryDate\": \"" << formatDate(node->expiryDate) << "\",\n";
            file << "    \"storageCenter\": \"" << escapeJSON(node->storageCenter) << "\"\n";
            file << "  }";

            traverse(node->right, first);
        };

        bool first = true;
        traverse(cropBSTRoot, first);

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << cropCount << " crops to data/crops.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save crops: " << e.what() << "\n";
    }
}

void loadCropsFromJSON() {
    try {
        ifstream file("data/crops.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing crops.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"cropId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            Crop* newCrop = new Crop;
            newCrop->cropId = stoi(extractJSONValue(obj, "cropId", false));
            newCrop->farmerId = stoi(extractJSONValue(obj, "farmerId", false));
            newCrop->cropType = extractJSONValue(obj, "cropType", true);
            newCrop->quantity = stoi(extractJSONValue(obj, "quantity", false));
            newCrop->storageQuantity = stoi(extractJSONValue(obj, "storageQuantity", false));
            newCrop->quality = extractJSONValue(obj, "quality", true);
            newCrop->pricePerKg = stoi(extractJSONValue(obj, "pricePerKg", false));
            newCrop->available = extractJSONValue(obj, "available", false) == "true";
            newCrop->storageCenter = extractJSONValue(obj, "storageCenter", true);
            newCrop->left = newCrop->right = nullptr;

            // Parse dates
            string dateAddedStr = extractJSONValue(obj, "dateAdded", true);
            newCrop->dateAdded = {stoi(dateAddedStr.substr(0, 2)), stoi(dateAddedStr.substr(3, 2)), stoi(dateAddedStr.substr(6, 4))};
            
            string expiryDateStr = extractJSONValue(obj, "expiryDate", true);
            newCrop->expiryDate = {stoi(expiryDateStr.substr(0, 2)), stoi(expiryDateStr.substr(3, 2)), stoi(expiryDateStr.substr(6, 4))};

            cropBSTRoot = insertCropBST(cropBSTRoot, newCrop);
            cropCount++;
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " crops from data/crops.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load crops: " << e.what() << "\n";
    }
}

/* ==================== ORDERS ==================== */

void saveOrdersToJSON() {
    try {
        ofstream file("data/orders.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/orders.json for writing.\n";
            return;
        }

        file << "[\n";
        Order* curr = orderHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"orderId\": " << curr->orderId << ",\n";
            file << "    \"cropId\": " << curr->cropId << ",\n";
            file << "    \"buyerId\": " << curr->buyerId << ",\n";
            file << "    \"farmerId\": " << curr->farmerId << ",\n";
            file << "    \"quantity\": " << curr->quantity << ",\n";
            file << "    \"farmerApproved\": " << (curr->farmerApproved ? "true" : "false") << ",\n";
            file << "    \"paid\": " << (curr->paid ? "true" : "false") << ",\n";
            file << "    \"delivered\": " << (curr->delivered ? "true" : "false") << ",\n";
            file << "    \"orderDate\": \"" << formatDate(curr->orderDate) << "\"\n";
            file << "  }";

            curr = curr->next;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " orders to data/orders.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save orders: " << e.what() << "\n";
    }
}

void loadOrdersFromJSON() {
    try {
        ifstream file("data/orders.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing orders.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"orderId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            Order* newOrder = new Order;
            newOrder->orderId = stoi(extractJSONValue(obj, "orderId", false));
            newOrder->cropId = stoi(extractJSONValue(obj, "cropId", false));
            newOrder->buyerId = stoi(extractJSONValue(obj, "buyerId", false));
            newOrder->farmerId = stoi(extractJSONValue(obj, "farmerId", false));
            newOrder->quantity = stoi(extractJSONValue(obj, "quantity", false));
            newOrder->farmerApproved = extractJSONValue(obj, "farmerApproved", false) == "true";
            newOrder->paid = extractJSONValue(obj, "paid", false) == "true";
            newOrder->delivered = extractJSONValue(obj, "delivered", false) == "true";
            newOrder->next = nullptr;

            addOrder(newOrder);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " orders from data/orders.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load orders: " << e.what() << "\n";
    }
}

/* ==================== TRANSPORT REQUESTS ==================== */

void saveTransportRequestsToJSON() {
    try {
        ofstream file("data/transport_requests.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/transport_requests.json for writing.\n";
            return;
        }

        file << "[\n";
        TransportRequest* curr = transportHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"requestId\": " << curr->requestId << ",\n";
            file << "    \"weight\": " << curr->weight << ",\n";
            file << "    \"distance\": " << curr->distance << ",\n";
            file << "    \"budget\": " << curr->budget << ",\n";
            file << "    \"organization\": \"" << escapeJSON(curr->organization) << "\",\n";
            file << "    \"accepted\": " << (curr->accepted ? "true" : "false") << ",\n";
            file << "    \"rejected\": " << (curr->rejected ? "true" : "false") << ",\n";
            file << "    \"completed\": " << (curr->completed ? "true" : "false") << ",\n";
            file << "    \"requesterId\": " << curr->requesterId << "\n";
            file << "  }";

            curr = curr->next;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " transport requests to data/transport_requests.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save transport requests: " << e.what() << "\n";
    }
}

void loadTransportRequestsFromJSON() {
    try {
        ifstream file("data/transport_requests.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing transport_requests.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"requestId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            TransportRequest* newReq = new TransportRequest;
            newReq->requestId = stoi(extractJSONValue(obj, "requestId", false));
            newReq->weight = stoi(extractJSONValue(obj, "weight", false));
            newReq->distance = stoi(extractJSONValue(obj, "distance", false));
            newReq->budget = stoi(extractJSONValue(obj, "budget", false));
            newReq->organization = extractJSONValue(obj, "organization", true);
            newReq->accepted = extractJSONValue(obj, "accepted", false) == "true";
            newReq->rejected = extractJSONValue(obj, "rejected", false) == "true";
            newReq->completed = extractJSONValue(obj, "completed", false) == "true";
            newReq->requesterId = stoi(extractJSONValue(obj, "requesterId", false));
            newReq->next = nullptr;

            addTransportRequest(newReq);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " transport requests from data/transport_requests.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load transport requests: " << e.what() << "\n";
    }
}

/* ==================== STORAGE REQUESTS ==================== */

void saveStorageRequestsToJSON() {
    try {
        ofstream file("data/storage_requests.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/storage_requests.json for writing.\n";
            return;
        }

        file << "[\n";
        StorageRequest* curr = storageHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"requestId\": " << curr->requestId << ",\n";
            file << "    \"cropId\": " << curr->cropId << ",\n";
            file << "    \"quantity\": " << curr->quantity << ",\n";
            file << "    \"budget\": " << curr->budget << ",\n";
            file << "    \"pricePerKg\": " << curr->pricePerKg << ",\n";
            file << "    \"cropName\": \"" << escapeJSON(curr->cropName) << "\",\n";
            file << "    \"organization\": \"" << escapeJSON(curr->organization) << "\",\n";
            file << "    \"accepted\": " << (curr->accepted ? "true" : "false") << ",\n";
            file << "    \"rejected\": " << (curr->rejected ? "true" : "false") << ",\n";
            file << "    \"requesterId\": " << curr->requesterId << "\n";
            file << "  }";

            curr = curr->next;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " storage requests to data/storage_requests.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save storage requests: " << e.what() << "\n";
    }
}

void loadStorageRequestsFromJSON() {
    try {
        ifstream file("data/storage_requests.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing storage_requests.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"requestId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            StorageRequest* newReq = new StorageRequest;
            newReq->requestId = stoi(extractJSONValue(obj, "requestId", false));
            newReq->cropId = stoi(extractJSONValue(obj, "cropId", false));
            newReq->quantity = stoi(extractJSONValue(obj, "quantity", false));
            newReq->budget = stoi(extractJSONValue(obj, "budget", false));
            newReq->pricePerKg = stoi(extractJSONValue(obj, "pricePerKg", false));
            newReq->cropName = extractJSONValue(obj, "cropName", true);
            newReq->organization = extractJSONValue(obj, "organization", true);
            newReq->accepted = extractJSONValue(obj, "accepted", false) == "true";
            newReq->rejected = extractJSONValue(obj, "rejected", false) == "true";
            newReq->requesterId = stoi(extractJSONValue(obj, "requesterId", false));
            newReq->next = nullptr;

            addStorageRequest(newReq);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " storage requests from data/storage_requests.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load storage requests: " << e.what() << "\n";
    }
}

/* ==================== VEHICLES ==================== */

void saveVehiclesToJSON() {
    try {
        ofstream file("data/vehicles.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/vehicles.json for writing.\n";
            return;
        }

        file << "[\n";
        Vehicle* curr = vehicleHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"vehicleId\": " << curr->vehicleId << ",\n";
            file << "    \"capacity\": " << curr->capacity << ",\n";
            file << "    \"available\": " << (curr->available ? "true" : "false") << ",\n";
            file << "    \"type\": \"" << escapeJSON(curr->type) << "\",\n";
            file << "    \"organization\": \"" << escapeJSON(curr->organization) << "\"\n";
            file << "  }";

            curr = curr->next;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " vehicles to data/vehicles.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save vehicles: " << e.what() << "\n";
    }
}

void loadVehiclesFromJSON() {
    try {
        ifstream file("data/vehicles.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing vehicles.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"vehicleId\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            Vehicle* newVehicle = new Vehicle;
            newVehicle->vehicleId = stoi(extractJSONValue(obj, "vehicleId", false));
            newVehicle->capacity = stoi(extractJSONValue(obj, "capacity", false));
            newVehicle->available = extractJSONValue(obj, "available", false) == "true";
            newVehicle->type = extractJSONValue(obj, "type", true);
            newVehicle->organization = extractJSONValue(obj, "organization", true);
            newVehicle->next = nullptr;

            addVehicle(newVehicle);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " vehicles from data/vehicles.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load vehicles: " << e.what() << "\n";
    }
}

/* ==================== STORAGE CENTERS ==================== */

void saveStorageCentersToJSON() {
    try {
        ofstream file("data/storage_centers.json");
        if (!file.is_open()) {
            cerr << "[JSON ERROR] Cannot open data/storage_centers.json for writing.\n";
            return;
        }

        file << "[\n";
        StorageCenter* curr = storageCenterHead;
        bool first = true;
        int count = 0;

        while (curr != nullptr) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n";
            file << "    \"organization\": \"" << escapeJSON(curr->organization) << "\",\n";
            file << "    \"totalCapacity\": " << curr->totalCapacity << ",\n";
            file << "    \"availableCapacity\": " << curr->availableCapacity << ",\n";
            file << "    \"temperature\": " << fixed << setprecision(1) << curr->temperature << ",\n";
            file << "    \"location\": \"" << escapeJSON(curr->location) << "\",\n";
            file << "    \"pricePerKg\": " << curr->pricePerKg << "\n";
            file << "  }";

            curr = curr->next;
            count++;
        }

        file << "\n]\n";
        file.close();
        cout << "[JSON] Saved " << count << " storage centers to data/storage_centers.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to save storage centers: " << e.what() << "\n";
    }
}

void loadStorageCentersFromJSON() {
    try {
        ifstream file("data/storage_centers.json");
        if (!file.is_open()) {
            cout << "[JSON] No existing storage_centers.json found. Starting fresh.\n";
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string json = buffer.str();
        file.close();

        size_t pos = 0;
        int count = 0;
        while ((pos = json.find("\"organization\"", pos)) != string::npos) {
            size_t objStart = json.rfind('{', pos);
            size_t objEnd = json.find('}', pos);
            if (objStart == string::npos || objEnd == string::npos) break;

            string obj = json.substr(objStart, objEnd - objStart + 1);

            StorageCenter* newCenter = new StorageCenter;
            newCenter->organization = extractJSONValue(obj, "organization", true);
            newCenter->totalCapacity = stoi(extractJSONValue(obj, "totalCapacity", false));
            newCenter->availableCapacity = stoi(extractJSONValue(obj, "availableCapacity", false));
            newCenter->temperature = stof(extractJSONValue(obj, "temperature", false));
            newCenter->location = extractJSONValue(obj, "location", true);
            newCenter->pricePerKg = stoi(extractJSONValue(obj, "pricePerKg", false));
            newCenter->next = nullptr;

            addStorageCenter(newCenter);
            count++;
            pos = objEnd + 1;
        }

        cout << "[JSON] Loaded " << count << " storage centers from data/storage_centers.json\n";
    } catch (const exception& e) {
        cerr << "[JSON ERROR] Failed to load storage centers: " << e.what() << "\n";
    }
}

/* ==================== MASTER FUNCTIONS ==================== */

void loadAllData() {
    cout << "\n=============== LOADING DATA FROM JSON ===============\n";
    ensureDataFolder();
    loadUsersFromJSON();
    loadCropsFromJSON();
    loadOrdersFromJSON();
    loadTransportRequestsFromJSON();
    loadStorageRequestsFromJSON();
    loadVehiclesFromJSON();
    loadStorageCentersFromJSON();
    cout << "=====================================================\n\n";
}

void saveAllData() {
    cout << "\n=============== SAVING DATA TO JSON ===============\n";
    ensureDataFolder();
    saveUsersToJSON();
    saveCropsToJSON();
    saveOrdersToJSON();
    saveTransportRequestsToJSON();
    saveStorageRequestsToJSON();
    saveVehiclesToJSON();
    saveStorageCentersToJSON();
    cout << "=================================================\n\n";
}
