#include <windows.h>
#include <cstdlib>
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
	if (initialize() != 0) {
		return 1;
	}

    // launch helper
	// eventually this will be modified to specifically select the thread and move it to part of process parent
	// this is each task will be run an two different tjreads and the results compared.
	ThreadPool pool;
	std::vector<std::future<void>> futures;
//	for (int i=0; i<20; ++i)
//		futures.push_back(
//			pool.enqueue([i]  {
//			std::cout << "Hello from worker thread " + std::to_string(i) + "\n";
//			}));
	
//	f.get();




	// process parent
    // save game state to database
    // cleanup
    // exit

	futures.push_back(
		pool.enqueue([]  {
		constexpr int worker=5;
		std::cout << "Hello from worker thread " + std::to_string(worker) + "\n";
		}));


	std::cout << "Press Enter to exit...\n";
	std::cin.get();
	return 0;
}

int initialize() {
	// Initialize game state, load resources, etc.
	std::cout << "Initializing Solitaire..." << std::endl;

	// open SQLite database
	if (!db_open("Solitaire.db")) {
		std::cerr << "Could not open database.\n";
		return 1;
	}
	std::cout << "SQLite opened successfully.\n";

	// the check for active deals is here. if no active deals, then a new deal is needed
	// if deals exist, then start worker threads.
	const char* cSql="select count(*) from Deal_Head where EndTime=0";
	std::cout << cSql << "\n";
	sqlite3_stmt* stmt = db_query(cSql);
	if (!stmt) {
		std::cerr << "Could not prepare startup query: "
				  << sqlite3_errmsg(g_db) << "\n";
		db_close();
		return 1;
	}

	int rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		int count = sqlite3_column_int(stmt, 0);
		std::cout << "Active deals: " << count << "\n";
		if (count == 0) {
			// create new deal
			if (makeNewDeal() != 0) {
				db_finalize(stmt);
				db_close();
				return 1;
			}
		}
	} else {
		std::cerr << "SQLite error: " << sqlite3_errmsg(g_db) << "\n";
		db_finalize(stmt);
		db_close();
		return 1;
	}

	db_finalize(stmt);
	return 0;
}

int makeNewDeal()

{
	if (!db_exec("BEGIN IMMEDIATE")) {
		return -1;
	}

	auto fail = [] {
		db_exec("ROLLBACK");
		return -1;
	};

	// deck_0.initialize
	std::cout << "Generating New Deal . . .\n";

	// update card_0 rnd to new rnd
	const char* cSql = "update card_0 set rnd=abs(random())";
	bool OK = db_exec(cSql);
	if (!OK) {
		return fail();
	}
	std::cout << "Sucessful shuffle.\n";

	// card_0 order by rnd
	cSql = "update deck_0 set card = (select cardid from (select cardid , row_number() over (order by rnd)-1 as r from card_0) where r=deck_0.pos)";
	OK = db_exec(cSql);

	if (!OK) {
		return fail();
	}

	// get next deal_no
	// create new deck_head
	// Deal_Head.DealNo is primary_key and thus auto increments.
	cSql="insert into Deal_Head (StartTime,Status) Values (datetime('now'),0)";
	sqlite3_int64 recordNum = insert_and_get_rowid(g_db,cSql);
	if (recordNum < 0) {
		return fail();
	}

	// i have deal number
	// create entry in deckHead
	std::string sSql="insert into Decks_Head (DealID, Start, Status) Values (" +
		std::to_string(recordNum) + ", datetime('now'), 0)";
	recordNum = insert_and_get_rowid(g_db,sSql);
	if (recordNum < 0) {
		return fail();
	}

	// deck_0.populate from card_0
	sSql="INSERT INTO Decks_data (deckID, card, place, seq, updown, pos) "
		"SELECT " + std::to_string(recordNum) + ", card, place, seq, updown, pos "
		"FROM deck_0";
	recordNum = insert_and_get_rowid(g_db,sSql);
	if (recordNum < 0) {
		return fail();
	}

	// copy deck_0 to Deck_data
	// return new deck_id
	if (!db_exec("COMMIT")) {
		db_exec("ROLLBACK");
		return -1;
	}


	return 0;
}
