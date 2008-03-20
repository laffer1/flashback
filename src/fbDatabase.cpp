/* $Id: fbDatabase.cpp,v 1.2 2008/03/20 18:46:13 wyverex Exp $ */

#include "fbDatabase.h"

/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/


fbDatabase::fbDatabase(fbData* _data, string path):data(_data), cs(), db(errlog)
{
	db.connect(path);
}

fbDatabase::fbDatabase(fbData* _data, const char* path):data(_data), cs(), db(errlog)
{
	db.connect(path);
}

fbDatabase::~fbDatabase()
{
	db.close();
}







