/* $Id: fbSQL.cpp,v 1.2 2008/03/14 20:11:36 wyverex Exp $ */

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

int fbSQL::exec(char* command,int(*callback)(void*,int,char**,char**))
{
	int ret = 0;
	char* errmsg;
	string err = "sqlite3 error code: ";

	fbLock lock(cs);

	ret = sqlite3_exec(db, command, callback, 0 , &errmsg);

	if(ret != SQLITE_OK)
	{
		err += ret;
		Error.print(WARN, SQLEXECERROR, err);
		sqlite3_free(errmsg);
	}

	return ret;
}

int fbSQL::exec(string command,int(*callback)(void*,int,char**,char**) )
{
	return exec(command.c_str(), callback);
}

