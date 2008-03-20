/* $Id: fbSQL.h,v 1.4 2008/03/20 18:46:13 wyverex Exp $ */


#ifndef fbSQL_H
#define fbSQL_H

#include "global.h"
#include "fbData.h"
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
 	fbSQL(fbData* _data);  /// < constructor
	~fbSQL();  /// < destructor

	void connect(char* database);   /// < connect to file
	void connect(string database);  /// < connect to file 
	void close();  /// < close open file

	bool isConnected();  /// < are we connected

	int exec(char* command,int(*callback)(void*,int,char**,char**));  /// <exec quarry
	int exec(string command,int(*callback)(void*,int,char**,char**)); /// < execute quarry


private:
	fbData* data;	/// < error logger
	fbCriticalSection cs;   /// < critical section
	sqlite3* db;		/// < database
	bool open;		/// < open bool
};

#endif


