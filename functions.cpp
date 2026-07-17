#include <windows.h>
#include <iostream>
#include "database.h"
#include <sstream>
#include <intrin.h>

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
