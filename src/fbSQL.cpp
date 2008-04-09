/* $Id: fbSQL.cpp,v 1.15 2008/04/09 15:27:20 wyverex Exp $ */

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
fbSQL::fbSQL(fbErrorLogger* log): errlog(log), cs(), qCS(), db(NULL), open(false), _rows(0), _cols(0)
{
	errlog->debug(NONE, "fbSQL.this");
}


/**
*  fbSQL
*  constructor
*  @param errlog Pointer to main error logger
*/
fbSQL::fbSQL(fbErrorLogger* log, sqlite3* database): errlog(log), cs(), qCS(), db(database), open(false), _rows(0), _cols(0)
{
	errlog->debug(NONE, "fbSQL.this");
}


/**
*  fbSQL
*  constructor
*  @param errlog Pointer to main error logger
*/
fbSQL::fbSQL(fbSQL& sql): errlog(sql.errlog), cs(), qCS(), db(sql.db), open(false), _rows(0), _cols(0)
{
	errlog->debug(NONE, "fbSQL.this");
}

/// Destructor
fbSQL::~fbSQL()
{
	errlog->debug(NONE, "fbSQL.~this");
	close();
}

/**
*  connect
*   connects to a database file
*  @param database File to open
*/
void fbSQL::connect(const char* database)
{
	errlog->debug(NONE, "fbSQL.connect");
	errlog->debug(NONE, database);
	
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
			errlog->err(FAILEDTOOPENDB, err);
			return;
		}
	}
	//were open, mark it
	open = true;

	//force table creation 
	//string cmd = "CREATE TABLE backup (ID INTEGER PRIMARY KEY, desc TEXT, date INTEGER, time INTEGER, repeatmode INTEGER, repeatval INTEGER, disk TEXT);";
	//exe(cmd);
	//cmd = "CREATE TABLE restore(ID INTEGER PRIMARY KEY, tarfile TEXT, path TEXT);";
	//exe(cmd);

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
*  exe
*  exectues an command on the database
*  @param command SQL command to send to DB
*/
int fbSQL::exe(string& cmd)
{
int ret = 0;
	char* errmsg;
	string err = "sqlite3 error code: ";
	// lock so two commands can't be sent at once
	fbLock lock(cs);

	// send command
	errlog->debug(NONE, cmd);
	ret = sqlite3_exec(db, cmd.c_str(), NULL, 0 , &errmsg);

	// test if okay
	if(ret != SQLITE_OK)
	{	//error, report warning
		err += ret;
		errlog->warn(SQLEXECERROR, err);
		errlog->warn(SQLEXECERROR, errmsg);
		sqlite3_free(errmsg);
	}

	//done, after return
	return ret;
}


int fbSQL::querry(string& cmd)
{
	int ret = 0;
	char* errmsg;
	char** result;
	string err = "sqlite3 error code: ";
	
	if(db == NULL)
	{
		errlog->warn(FAILEDTOOPENDB, "fbSQL: Querry on none opened database");
		return -1;
	}

	// lock so two commands can't be sent at once
	fbLock lock(cs);
	qCS.lock();   /// < lock querry

	// send command
	//ret = sqlite3_exec(db, command, callback, 0 , &errmsg);
	ret = sqlite3_get_table(db, cmd.c_str(), &result, &_rows, &_cols, &errmsg);

	// test if okay
	if(ret != SQLITE_OK)
	{	//error, report warning
		err += errmsg;
		errlog->warn(SQLEXECERROR, err);
		sqlite3_free(errmsg);
		return ret;
	}


	char buff[500];
	sprintf(buff, "found %d rows with %d cols",  _rows, _cols);
	string msg = buff;	
	errlog->debug(NONE, msg);
	

	//clear vectors
	if(col_header.size() != 0) col_header.clear();
	if(table.size() != 0) table.clear();

	//save header information
	for(int i = 0; i < _cols; ++i)
		col_header.push_back(result[i]);

	//save data
	for(int i = 0; i < _cols*_rows; ++i)
		if(result[_cols+i] != NULL)
			table.push_back(result[_cols+i]);
		else
			table.push_back("");


	sprintf(buff, "filled Table data: %d", (int)table.size());
	msg = buff;	
	errlog->debug(NONE, msg);

	sqlite3_free_table(result);
	errlog->debug(NONE, "fbSQL: Querry OK");
	return ret;
}


