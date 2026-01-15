#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <string>
#include <vector>

using namespace std;

// Function declarations for JSON file handling

// Create data folder if it doesn't exist
void ensureDataFolder();

// Save functions
void saveUsersToJSON();
void saveCropsToJSON();
void saveOrdersToJSON();
void saveTransportRequestsToJSON();
void saveStorageRequestsToJSON();
void saveVehiclesToJSON();
void saveStorageCentersToJSON();

// Load functions
void loadUsersFromJSON();
void loadCropsFromJSON();
void loadOrdersFromJSON();
void loadTransportRequestsFromJSON();
void loadStorageRequestsFromJSON();
void loadVehiclesFromJSON();
void loadStorageCentersFromJSON();

// Load all data at startup
void loadAllData();

// Save all data
void saveAllData();

#endif // JSON_HANDLER_H
