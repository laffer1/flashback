/* $Id: fbDatabase.h,v 1.6 2008/04/08 19:16:46 wyverex Exp $ */

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

	bool querryBackups();
	bool getBackupRow(string& desc, fbDate& date, fbTime& time, string& path, 
		Repeat_type* rt, int* rv, int* id);
	

private:
	fbErrorLogger* errlog;
	fbCriticalSection cs;
	fbSQL db;

	int row;
};
#endif


