#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

// Opens the database file (creates handle)
bool db_open(const char* filename);

// Closes the database
void db_close();

// Runs a SQL statement that does not return rows
bool db_exec(const char* sql);

// Prepares a SELECT and returns sqlite3_stmt*
sqlite3_stmt* db_query(const char* sql);

// Finalizes a prepared statement
void db_finalize(sqlite3_stmt* stmt);

// Returns the global db handle
sqlite3* db_handle();

#endif
