/* $Id: fbSQL.h,v 1.2 2008/03/14 20:11:36 wyverex Exp $ */


#ifndef fbSQL_H
#define fbSQL_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbLock.h"

//requires libsqlite3-dev
#include <sqlite3.h>

typedef int callback(void*,int, char**, char**); 

class fbSQL
{
public:
 	fbSQL(fbErrorLogger& errlog);
	~fbSQL();

	void connect(char* database);
	void connect(string database);
	void close();

	bool isConnected();

	int exec(char* command,int(*callback)(void*,int,char**,char**));
	int exec(string command,int(*callback)(void*,int,char**,char**));	


private:
	fbErrorLogger Error;
	fbCriticalSection cs;
	sqlite3* db;
	bool open;
};

#endif


