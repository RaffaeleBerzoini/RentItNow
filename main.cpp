#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    char* errMsg = 0;

    // Open a database connection
    if (sqlite3_open("example.db", &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    std::cout << "Opened database successfully!" << std::endl;

    // Create a SQL table
    const char* sql = "CREATE TABLE IF NOT EXISTS PERSON("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "NAME TEXT NOT NULL);";

    // Execute SQL statement
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully!" << std::endl;
    }

    // Close the database connection
    sqlite3_close(db);
    return 0;
}
