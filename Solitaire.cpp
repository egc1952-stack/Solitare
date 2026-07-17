#include <windows.h>
#include <intrin.h>
#include <iostream>
#include <sstream>
#include <synchapi.h>
#include <vector>
#include "database.h"
#include "functions.h"
#include "Solitaire.h"
#include "sqlite3.h"





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

int initialize() {
	sqlite3* db = nullptr;
	// Initialize game state, load resources, etc.
	std::cout << "Initializing Solitaire..." << std::endl;

	// open SQLite database
	db_open("Solitaire.db");
	std::cout << "SQLite opened successfully.\n";

	// the check for active deals is here. if no active deals, then a new deal is needed
	// if deals exist, then start worker threads.
	const char* sql="select count(*) from Deal_Head where EndTime=0";
	std::cout << sql << "\n";
	sqlite3_stmt* stmt = db_query(sql);
	int rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		int count = sqlite3_column_int(stmt, 0);
		std::cout << "Active deals: " << count << "\n";
		if (count == 0) {
			// create new deal
			int result = makeNewDeal();
			return 0;
		}
	else {
		std::cout << "SQLite error: " << rc << "\n";
		return 1;
	}	
}	
	

	

return 0;
}

int makeNewDeal()

{
	// deck_0.initialize
	std::cout << "Generating New Deal . . .\n";

	// update card_0 rnd to new rnd
	const char* sql = "update card_0 set rnd=abs(random())";
	bool OK = db_exec(sql);
	if (!OK) {
		__debugbreak();
		return -1;
	}
	std::cout << "Sucessful shuffle.\n";

	// card_0 order by rnd
	sql = "update deck_0 set card = (select cardid from (select cardid , row_number() over (order by rnd)-1 as r from card_0) where r=deck_0.pos)";
	OK = db_exec(sql);

	if (!OK) {
		__debugbreak();
		return -1;
	}

	// get next deal_no
	// create new deck_head
	// Deal_Head.DealNo is primary_key and thus auto increments.
	sql="insert into Deal_Head DEFAULT VALUES";
	db_exec(sql);
	sqlite3_int64 dealnoRow = sqlite3_last_insert_rowid(g_db);
	
	std::stringstream ss;
	ss << "select DealNo from Deal_Head where DealNo = " << dealnoRow;
	sqlite3_stmt* stmt = nullptr;
	int rc = sqlite3_prepare_v2(g_db, ss.str().c_str(), -1, &stmt, nullptr);
	int iDealNo = -1;
	if (rc == SQLITE_OK) {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW){
			iDealNo = sqlite3_column_int(stmt,0);
		}
	}
	if (iDealNo==-1){
		__debugbreak();
		std::cerr << "Fatal error: could not read DealNo\n";
		std::exit(1);          // stops program in release mode
	}


	// deck_0.populate from card_0
	// copy deck_0 to Deck_data
	// return new neck_id


	return 0;
}
