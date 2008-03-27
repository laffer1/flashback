/* $Id: fbDatabase.cpp,v 1.4 2008/03/27 17:48:14 wyverex Exp $ */

#include "fbDatabase.h"

/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/


fbDatabase::fbDatabase(fbErrorLogger* log, string path):errlog(log), cs(), db(errlog)
{
	db.connect(path);
	errlog->debug(NONE, "fbDatabase.this");
}

fbDatabase::fbDatabase(fbErrorLogger* log, const char* path):errlog(log), cs(), db(errlog)
{
	db.connect(path);
	errlog->debug(NONE, "fbDatabase.this");
}

fbDatabase::~fbDatabase()
{
	db.close();
	errlog->debug(NONE, "fbDatabase.~this");
}







