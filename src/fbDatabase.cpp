/* $Id: fbDatabase.cpp,v 1.1 2008/03/16 17:05:45 wyverex Exp $ */

#include "fbDatabase.h"

/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/


fbDatabase::fbDatabase(fbErrorLogger errlog, string path):Error(errlog), cs(), db(errlog)
{
	db.connect(path);
}

fbDatabase::fbDatabase(fbErrorLogger errlog, char* path):Error(errlog), cs(), db(errlog)
{
	db.connect(path);
}

fbDatabase::~fbDatabase()
{
	db.close();
}







