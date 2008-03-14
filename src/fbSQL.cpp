/* $Id: fbSQL.cpp,v 1.1 2008/03/14 19:50:09 wyverex Exp $ */

#include "fbSQL.h"

fbSQL::fbSQL(fbErrorLogger& errlog): Error(errlog), cs(), db(NULL), open(false)
{
}

fbSQL::~fbSQL()
{
	close();
}

void fbSQL::connect(char* database)
{
	fbLock lock(cs);
	string err = "File: ";

	if(!open)
	{
		int ret = sqlite3_open(database, &db);
		if(ret)
		{
			err = database;
			Error.print(ERR, FAILEDTOOPENDB, err);
			return;
		}
	}
	open = true;
}


void fbSQL::connect(string database)
{
	connect(database.c_str());
}


void fbSQL::close()
{
	fbLock lock(cs);

	if(open)
		sqlite3_close(db);
	open = false;
}

bool fbSQL::isConnected()
{
	return open;
}

