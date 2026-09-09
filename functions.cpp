#include <windows.h>
#include <iostream>
#include "database.h"
#include <sstream>
#include <intrin.h>

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
	const char* sql = "update card_0 set rnd=abs(random())";
	bool OK = db_exec(sql);
	if (!OK) {
		return fail();
	}
	std::cout << "Sucessful shuffle.\n";

	// card_0 order by rnd
	sql = "update deck_0 set card = (select cardid from (select cardid , row_number() over (order by rnd)-1 as r from card_0) where r=deck_0.pos)";
	OK = db_exec(sql);

	if (!OK) {
		return fail();
	}

	// get next deal_no
	// create new deck_head
	// Deal_Head.DealNo is primary_key and thus auto increments.
	sql="insert into Deal_Head DEFAULT VALUES";
	if (!db_exec(sql)) {
		return fail();
	}

	sqlite3_int64 dealnoRow = sqlite3_last_insert_rowid(g_db);
	if (dealnoRow < 0) {
		return fail();
	}
	
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
		std::cerr << "Fatal error: could not read DealNo\n";
		if (stmt) sqlite3_finalize(stmt);
		return fail();
	}
	sqlite3_finalize(stmt);

	// create entry in deck head
	ss.str("");
	ss.clear();
	ss << "insert into Decks_Head (DealID, Start, Status) values ("
		<< iDealNo << ", datetime('now'), 0)";
	if (!db_exec(ss.str().c_str())) {
		return fail();
	}

	sqlite3_int64 deckRow = sqlite3_last_insert_rowid(g_db);
	if (deckRow < 0) {
		return fail();
	}


	// deck_0.populate from card_0
	// copy deck_0 to Deck_data
	// return new neck_id
	ss.str("");
	ss.clear();
	ss << "insert into Decks_data (deckID, card, place, seq, updown, pos) "
		<< "select " << deckRow << ", card, place, seq, updown, pos "
		<< "from deck_0";
	if (!db_exec(ss.str().c_str())) {
		return fail();
	}

	if (!db_exec("COMMIT")) {
		db_exec("ROLLBACK");
		return -1;
	}


	return 0;
}
