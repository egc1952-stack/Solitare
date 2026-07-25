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
	const char* cSql="select count(*) from Deal_Head where EndTime=0";
	std::cout << cSql << "\n";
	sqlite3_stmt* stmt = db_query(cSql);
	int rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		int count = sqlite3_column_int(stmt, 0);
		std::cout << "Active deals: " << count << "\n";
		if (count == 0) {
			// create new deal
			int result = makeNewDeal();
			return 0;
		}
	} else {
		std::cout << "SQLite error: " << rc << "\n";
		return 1;
	}	

	

	

return 0;
}

int makeNewDeal()

{
	// deck_0.initialize
	std::cout << "Generating New Deal . . .\n";

	// update card_0 rnd to new rnd
	const char* cSql = "update card_0 set rnd=abs(random())";
	bool OK = db_exec(cSql);
	if (!OK) {
		__debugbreak();
		return -1;
	}
	std::cout << "Sucessful shuffle.\n";

	// card_0 order by rnd
	cSql = "update deck_0 set card = (select cardid from (select cardid , row_number() over (order by rnd)-1 as r from card_0) where r=deck_0.pos)";
	OK = db_exec(cSql);

	if (!OK) {
		__debugbreak();
		return -1;
	}

	// get next deal_no
	// create new deck_head
	// Deal_Head.DealNo is primary_key and thus auto increments.
	cSql="insert into Deal_Head (StartTime,Status) Values (datetime('now'),0)";
	sqlite3_int64 recordNum = insert_and_get_rowid(g_db,cSql);
	
	std::string sSql="select DealNo from Deal_Head where Dealno = " +std::to_string(recordNum);
	int iDealNo = exec_query_single_int(g_db, sSql);
	if (iDealNo==-1){
		__debugbreak();
		std::cerr << "Fatal error: could not read DealNo\n";
		std::exit(1);          // stops program in release mode
	}
	std::string sDealNo = std::to_string(iDealNo);

	// i have deal number
	// create entry in deckHead
	sSql="insert into Decks_Head (DealID, Start, Status) Values ('" + sDealNo + "', datetime('now'), 0)";
	recordNum = insert_and_get_rowid(g_db,sSql);
	sSql="select DeckID from Decks_Head where DeckID = " +std::to_string(recordNum);
	int iDeckNo = exec_query_single_int(g_db, sSql);
	if (iDeckNo==-1){
		__debugbreak();
		std::cerr << "Fatal error: could not read DeckNo\n";
		std::exit(1);          // stops program in release mode
	}
	std::string sDeckNo = std::to_string(iDeckNo);

	// deck_0.populate from card_0
	sSql="INSERT INTO Decks_data (deckID, card, place, seq, updown, pos) "
		"SELECT " + sDeckNo + ", card, place, seq, updown, pos "
		"FROM deck_0";
	recordNum = insert_and_get_rowid(g_db,sSql);

	
	






	// copy deck_0 to Deck_data
	// return new deck_id


	return 0;
}
