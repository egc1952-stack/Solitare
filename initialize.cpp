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
	int rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		int count = sqlite3_column_int(stmt, 0);
		std::cout << "Active deals: " << count << "\n";
	}
	else if (rc == SQLITE_DONE) {
		std::cout << "New Deal Needed.\n";
	}
	else {
		std::cout << "SQLite error: " << rc << "\n";
}	// create new deal
	

	

return 0;
}