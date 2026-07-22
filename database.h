#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <string>


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

// outputs the results of a select sql but returns nothing
void db_disp_query(const char* sql); 

extern sqlite3* g_db;

//execute query and returns single row
int exec_query_single_int(sqlite3* db, const std::string& sql);

// insert record and return rowID
sqlite3_int64 insert_and_get_rowid(sqlite3* db, const std::string& sql);


#endif
