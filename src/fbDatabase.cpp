/* $Id: fbDatabase.cpp,v 1.10 2008/04/09 00:24:17 wyverex Exp $ */

#include "fbDatabase.h"

/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/


fbDatabase::fbDatabase(fbErrorLogger* log, string path):errlog(log), cs(), db(errlog), row(0)
{
	db.connect(path);
	errlog->debug(NONE, "fbDatabase.this");
}

fbDatabase::fbDatabase(fbErrorLogger* log, const char* path):errlog(log), cs(), db(errlog), row(0)
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

bool fbDatabase::addRestoreJob(string& tarfile, string& dest)
{
	char buff[500];
	sprintf(buff,"insert into restore (tarfile, path) values (\'%s\', \'%s\');", tarfile.c_str(), dest.c_str());
	string cmd = buff;

	errlog->debug(NONE, cmd);
	if(!db.exe(cmd))
	{
		return false;
		errlog->warn(UNKNOWN, "Failed to add Restore Job..");
	}

//restore (ID INTEGER PRIMARY KEY, tarfile TEXT, path TEXT);
	
	return true;

}

bool fbDatabase::querryBackups()
{
	char buff[500];
	fbDate date;
	fbTime time;

	sprintf(buff,"select * from backup where date <= %ld AND time <= %ld;", 
	   date.getJulian(), time.getTicks());

	string cmd = buff;
	errlog->debug(NONE, cmd);

	db.querry(cmd);
	
	errlog->debug(NONE, "Querry Done");

	row = 0;

	return true;
}


bool fbDatabase::querryRestore()
{
	string cmd = "select * from restore;";
	errlog->debug(NONE, cmd);

	db.querry(cmd);
	errlog->debug(NONE, "Querry Done");

	row = 0;

	return true;
}


bool fbDatabase::getBackupRow(string& desc, fbDate& date, fbTime& time, string& path, 
		Repeat_type* rt, int* rv, int* id)
{
	errlog->debug(NONE, "fbDatabase:  Getting Backup Rows");

	if(row >= db.rows()) 
	{
		errlog->debug(NONE, "fbDatabase:  No Rows To Get");
		return false;
	}
	
	int index =  db.cols() * row;

	errlog->debug(NONE, "fbDatabase:  Found a ROW!");

	*id = atoi(db.table[index++].c_str());
	desc = db.table[index++];
	date.setJulian(atol(db.table[index++].c_str()));
	time.setTicks(atol(db.table[index++].c_str()));
	*rt = (Repeat_type)atoi(db.table[index++].c_str());
	*rv =  atoi(db.table[index++].c_str());
	path = db.table[index++];

	++row;
	errlog->debug(NONE, "fbDatabase:  Got Row");
	return true;
}


bool fbDatabase::getRestoreRow(string& tarfile, string& path, int* id)
{
	errlog->debug(NONE, "fbDatabase:  Getting Restore Rows");

	if(row >= db.rows()) 
	{
		errlog->debug(NONE, "fbDatabase:  No Rows To Get");
		return false;
	}
	
	int index =  db.cols() * row;

	errlog->debug(NONE, "fbDatabase:  Found a ROW!");

	*id = atoi(db.table[index++].c_str());
	tarfile = db.table[index++];
	path = db.table[index++];

	++row;
	errlog->debug(NONE, "fbDatabase:  Got Row");
	return true;
}



bool fbDatabase::deleteRow(const char* table, int id)
{
	char buff[500];
	sprintf(buff, "DELETE FROM %s WHERE ID = %d;", table, id);
	string cmd = buff;
	return db.exe(cmd);
}






