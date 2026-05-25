#include "Solitaire.h"
#include <iostream>
//#include <vector>
#include "sqlite3.h"
#include "database.h"


int initialize() {
    sqlite3* db = nullptr;

    // Initialize game state, load resources, etc.
				std::cout << "Initializing Solitaire..." << std::endl;
    // open SQLite database
    db_open("Solitaire.db");
    std::cout << "SQLite opened successfully.\n";
 
    // the check for active deals is here. if no active deals, then a new deal is needed
    // if deals exist, then start worker threads.
    const char*sql="select count(*) from Deal_Head where EndTime=0";
    std::cout << sql << "\n";
    sqlite3_stmt* stmt = db_query(sql);
    if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            // create new deal
            std::cout << "New Deal Needed.\n";

        }

   


    return 0;
}