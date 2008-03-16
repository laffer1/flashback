/* $Id: fbDatabase.h,v 1.1 2008/03/16 17:05:46 wyverex Exp $ */

#ifndef fbDATABASE_H
#define fbDATABASE_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbLock.h"
#include "fbSQL.h"
#include "fbDate.h"
#include "fbTime.h"
#include "fbRepeatType.h"


// todo:  Implment all SQL stuff!
//   add init function
//   needs settings access
//   needs schedule access
//   needs repository !


/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/
class fbDatabase
{
public:
	fbDatabase(fbErrorLogger errlog, string path);
	fbDatabase(fbErrorLogger errlog, char* path);
	~fbDatabase();

	///if the database doesn't exist yet, we should make it
	void init();


	///Schedule functions
	void createSchedule(string desc, fbDate date, fbTime time, Repeat_type repeat, int rint, string path);
	void createSchedule(char* desc, fbDate date, fbTime time, Repeat_type repeat, int rint, char* path);

	void updateSchedule(int id);  /// < runs repeat condition code... removes row or updates date/time values

	

	///Repository functions

	/// Settings functions
	//ints
	int getsetting_i(string name);
	int getsetting_i(char* name);
	void setsetting_i(string name, int val);
	void setsetting_i(char* name, int val);
	//floats
	float getsetting_f(string name);
	float getsetting_f(char* name);
	void  setsetting_f(string name, float val);
	void  setsetting_f(char* name, float val);
	//strings
	void getsetting_s(string name, string& val);
	void getsetting_s(string name, char* val, int length);
	void setsetting_s(string name, string val);
	void setsetting_s(string name, char* val, int length);
	void getsetting_s(char* name, string& val);
	void getsetting_s(char* name, char* val);
	void setsetting_s(char* name, string val);
	void setsetting_s(char* name, char* val);

private:
	fbErrorLogger Error;
	fbCriticalSection cs;
	fbSQL db;

};
#endif


