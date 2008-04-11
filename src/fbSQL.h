/* $Id: fbSQL.h,v 1.12 2008/04/11 02:21:15 ctubbsii Exp $ */


#ifndef fbSQL_H
#define fbSQL_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbLock.h"

//requires libsqlite3-dev
#include <sqlite3.h>

/**
*  fbSQL
*  Thread safe Wrapper for SQLite3
*  @author Byron Heads
*  @date March 14, 2008
*/

class fbSQL
{
public:
 	fbSQL(fbErrorLogger* log);  /// < constructor
 	fbSQL(fbErrorLogger* log, sqlite3* database);  /// < constructor, with database pointer
	fbSQL(fbSQL& sql);  /// < copy constructor
	~fbSQL();  /// < destructor

	void connect(const char* database);   /// < connect to file
	void connect(string database);  /// < connect to file
	void close();  /// < close open file

	bool isConnected();  /// < are we connected

	int exe(string& cmd);  /// < new command method
	int query(string& cmd);  /// < runs a query, locks qCS()
	void queryDone(); /// < unlock query


	const int rows() { return _rows; };
	const int cols() { return _cols; };



	vector<string> col_header;
	vector<string> table;

	private:
	fbErrorLogger* errlog;	/// < error logger
	fbCriticalSection cs;   /// < critical section
	fbCriticalSection qCS;   /// < critical section
	sqlite3* db;		/// < database
	bool open;		/// < open bool

	int _rows, _cols;	/// < save of table in memory
};

#endif


