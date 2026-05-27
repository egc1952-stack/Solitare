#include "Solitaire.h"
#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "functions.h"
#include <synchapi.h>



int junk   () {
        sqlite3* db = nullptr;

    // Initialize game state, load resources, etc.
    std::cout << "Initializing Solitaire..." << std::endl;
    // open SQLite database
     if (sqlite3_open("Solitaire.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << "\n";
        return 1;
    }
    std::cout << "SQLite opened successfully.\n";
    // shuffle card_0
    const char* selectSQL = "update card_0  set rnd= random()";
        

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Select prepare error: " << sqlite3_errmsg(db) << "\n";
        return 1;
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Update step error: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return 1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
 
    return 0; // Return 0 on success
}

int main() {
	Sleep(2000);
    int result = initialize();
    // load game state from database
    // launch helper
    // process parent
    // save game state to database
    // cleanup
    // exit
}
