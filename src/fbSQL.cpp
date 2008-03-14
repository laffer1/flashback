/* $Id: fbSQL.cpp,v 1.3 2008/03/14 20:32:26 wyverex Exp $ */

#include "fbSQL.h"


/**
* fbSQL
*  Thread safe wrapper for SQLite3
*  @author Byron Heads
*  @date March 14, 2008
*  @note depends on libsqlite3-dev
*/


/**
*  fbSQL
*  constructor
*  @param errlog Pointer to main error logger
*/
fbSQL::fbSQL(fbErrorLogger& errlog): Error(errlog), cs(), db(NULL), open(false)
{
}

/// Destructor
fbSQL::~fbSQL()
{
	close();
}

/**
*  connect
*   connects to a database file
*  @param database File to open
*/
void fbSQL::connect(char* database)
{
    	// lock for thread safety
	fbLock lock(cs);	
	//error string
	string err = "File: ";

	// if not open already
	if(!open)
	{
		// try to open
		int ret = sqlite3_open(database, &db);
		if(ret)
		{	//open failed report error
			err = database;
			Error.print(ERR, FAILEDTOOPENDB, err);
			return;
		}
	}
	//were open, mark it
	open = true;
}


/** 
*  connect
*  connect database, string overload
*  @param database String
*/
void fbSQL::connect(string database)
{
	connect(database.c_str());
}

/**
*  close
*  close connection to database
*/
void fbSQL::close()
{
	fbLock lock(cs);

	if(open)
		sqlite3_close(db);
	open = false;
}


/**
*  isConnected 
*  test if were connected to a database
*  @return True if connect passed
*/
bool fbSQL::isConnected()
{
	return open;
}

/**
*  exec
*  exectues a querry on the database
*  @param command SQL command to send to DB
*  @param callback SQLite sends result to callback
*  @return sqlite3_exec return value
*/
int fbSQL::exec(char* command,int(*callback)(void*,int,char**,char**))
{
	int ret = 0;
	char* errmsg;
	string err = "sqlite3 error code: ";
	
	// lock so two commands can't be sent at once
	fbLock lock(cs);

	// send command
	ret = sqlite3_exec(db, command, callback, 0 , &errmsg);

	// test if okay
	if(ret != SQLITE_OK)
	{	//error, report warning
		err += ret;
		Error.print(WARN, SQLEXECERROR, err);
		sqlite3_free(errmsg);
	}

	//done, unlocks here
	return ret;
}


/**
*  exec
*  exectues a querry on the database
*  overloaded string wrapper
*  @param command SQL command to send to DB
*  @param callback SQLite sends result to callback
*  @return sqlite3_exec return value
*/
int fbSQL::exec(string command,int(*callback)(void*,int,char**,char**) )
{
	return exec(command.c_str(), callback);
}

