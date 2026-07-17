#include "database.h"
#include <iostream>
#include "sqlite3.h"
#include "Solitaire.h"

sqlite3* g_db = nullptr;

sqlite3* db_handle() {
    return g_db;
}

bool db_open(const char* filename) {
    if (sqlite3_open(filename, &g_db) != SQLITE_OK) {
        std::cerr << "DB open error: "
                  << sqlite3_errmsg(g_db) << "\n";
        return false;
    }
    std::cout << "DB opened.\n";
    return true;
}

void db_close() {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = nullptr;
        std::cout << "DB closed.\n";
    }
}

bool db_exec(const char* sql) {
	std::cout << sql << "\n";
    char* err = nullptr;
    int rc = sqlite3_exec(g_db, sql, nullptr, nullptr, &err);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
}

sqlite3_stmt* db_query(const char* sql) {
	std::cout << sql << "\n";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(
        g_db, sql, -1, &stmt, nullptr
    );

    if (rc != SQLITE_OK) {
        std::cerr << "Prepare error: "
                  << sqlite3_errmsg(g_db) << "\n";
        return nullptr;
    }
    return stmt;
}

void db_disp_query(const char* sql) {
	sqlite3_stmt* stmt = nullptr;

	int rc = sqlite3_prepare_v2(
		g_db,
		sql,
		-1,
		&stmt,
		nullptr
	);

	if (rc != SQLITE_OK) {
		std::cerr << "Prepare: "
				<< sqlite3_errmsg(g_db)
				<< "\n";
		return;
	}

	int cols = sqlite3_column_count(stmt);
	std::cout << "cols = " << cols << "\n";

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0; i < cols; i++) {
			const unsigned char* txt =
				sqlite3_column_text(stmt, i);

			std::cout << txt << " ";
		}
		std::cout << "\n";
	}


	sqlite3_finalize(stmt);
}

void db_finalize(sqlite3_stmt* stmt) {
	if (stmt) sqlite3_finalize(stmt);
}
