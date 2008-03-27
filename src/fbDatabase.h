/* $Id: fbDatabase.h,v 1.4 2008/03/27 17:48:14 wyverex Exp $ */

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
	fbDatabase(fbErrorLogger* log, string path);
	fbDatabase(fbErrorLogger* log, const char* path);
	~fbDatabase();

	
	

private:
	fbErrorLogger* errlog;
	fbCriticalSection cs;
	fbSQL db;

};
#endif


