/* $Id: fbDatabase.h,v 1.8 2008/04/09 00:24:17 wyverex Exp $ */

#ifndef fbDATABASE_H
#define fbDATABASE_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbLock.h"
#include "fbSQL.h"
#include "fbDate.h"
#include "fbTime.h"
#include "fbRepeatType.h"


/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/
class fbDatabase
{
public:
	fbDatabase(fbErrorLogger* log, string path);
	fbDatabase(fbErrorLogger* log, const char* path);
	~fbDatabase();

	/// Add Back up job
	bool addBackupJob(string& desc, fbDate& date, fbTime& time, string& path, Repeat_type rt = ONCE, int rv = 0);
	bool addRestoreJob(string& tarfile, string& dest);

	bool querryBackups();
	bool querryRestore();
	bool getBackupRow(string& desc, fbDate& date, fbTime& time, string& path, 
		Repeat_type* rt, int* rv, int* id);
	bool getRestoreRow(string& tarfile, string& path, int* id);
	
	bool deleteRow(const char* table, int id);
	
	

private:
	fbErrorLogger* errlog;
	fbCriticalSection cs;
	fbSQL db;

	int row;
};
#endif


