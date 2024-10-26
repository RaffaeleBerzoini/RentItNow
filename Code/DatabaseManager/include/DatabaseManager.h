#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbFilePath);

	// Set-up the Database if it does not exist
    void SetupDB();

private:
    std::string dbFilePath; // Database file path
    void CreateTables(sqlite3* db);
};

#endif // DATABASE_MANAGER_H
