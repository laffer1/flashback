/* $Id: fbDatabase.cpp,v 1.6 2008/04/08 15:33:45 wyverex Exp $ */

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


bool fbDatabase::addBackupJob(string& desc, fbDate& date, fbTime& time, string& path, Repeat_type rt, int rv)
{
	char buff[500];
	sprintf(buff,"insert into backup (desc, date, time, repeatmode, repeatval, disk) values  (\'%s\',%ld,%ld,%d,%d,\'%s\');",
		desc.c_str(), date.getJulian(), time.getTicks(), rt, rv, path.c_str());
	string cmd = buff;

	errlog->debug(NONE, cmd);
	if(!db.exe(cmd))
	{
		return false;
		errlog->warn(UNKNOWN, "Failed to add Backup Job..");
	}

//backup (ID INTEGER PRIMARY KEY, desc TEXT, date INTEGER, time INTEGER, repeatmode INTEGER, repeatval INTEGER, disk TEXT);
	
	return true;
}






