/* $Id: fbDatabase.h,v 1.11 2008/04/18 04:50:54 laffer1 Exp $ */

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

	string & getBackupList(); /// Get a list of pending and current backups.

	/// Backup SQL
	bool addBackupJob(string& desc, fbDate& date, fbTime& time, string& path, Repeat_type rt = ONCE, int rv = 0);
	bool queryBackups();
	bool getBackupRow(string& desc, fbDate& date, fbTime& time, string& path,
		Repeat_type* rt, int* rv, int* id);

	/// Restore SQL
	bool addRestoreJob(string& tarfile, string& dest);
	bool getRestoreRow(string& tarfile, string& path, int* id);
	bool queryRestore();

	///Repo SQL
	bool addRepo(string& desc, fbDate& date, fbTime& time, string& path, string& tarfile);
	bool getRepoRow(string& desc, fbDate& date, fbTime& time, string& path, string& tarfile, int* id);
	bool queryRepo();


	/// Generic SQL
	bool deleteRow(const char* table, int id);

private:
	fbErrorLogger* errlog;
	fbCriticalSection cs;
	fbSQL db;

	int row;
};
#endif


