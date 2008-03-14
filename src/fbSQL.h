/* $Id: fbSQL.h,v 1.1 2008/03/14 19:50:09 wyverex Exp $ */


#ifndef fbSQL_H
#define fbSQL_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbLock.h"

//requires libsqlite3-dev
#include <sqlite3.h>

class fbSQL
{
public:
 	fbSQL(fbErrorLogger& errlog);
	~fbSQL();

	void connect(char* database);
	void connect(string database);
	void close();

	bool isConnected();

	
private:
	fbErrorLogger Error;
	fbCriticalSection cs;
	sqlite3* db;
	bool open;


};

#endif


