/* $Id: fbScheduler.h,v 1.5 2008/04/09 12:52:50 wyverex Exp $ */

#ifndef fbSCHEDULER_H
#define fbSCHEDULER_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include "fbBackup.h"

//#include <ctime>

/**
*  fbScheduler
*  Backup and restore Scheduler
*  @author Byron Heads
*  @date March 10, 2008
*/
class fbScheduler: public fbThread
{
public:
	fbScheduler(fbData* _data);
	~fbScheduler();

	void startup();
	void shutdown();

private:
	fbData* data;
	void run();
};

#endif

