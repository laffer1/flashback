/* $Id: fbDatabase.cpp,v 1.3 2008/03/20 19:07:48 wyverex Exp $ */

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
}

fbDatabase::fbDatabase(fbErrorLogger* log, const char* path):errlog(log), cs(), db(errlog)
{
	db.connect(path);
}

fbDatabase::~fbDatabase()
{
	db.close();
}







