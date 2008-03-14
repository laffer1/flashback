/* $Id: fbScheduler.h,v 1.2 2008/03/14 19:18:09 wyverex Exp $ */

#ifndef fbSCHEDULER_H
#define fbSCHEDULER_H

#include "global.h"
#include "fbThread.h"
#include "fbErrorLogger.h"

#include <ctime>

/**
*  fbScheduler
*  Backup and restore Scheduler
*  @author Byron Heads
*  @date March 10, 2008
*/
class fbScheduler: public fbThread
{
public:
	fbScheduler(fbErrorLogger& errlog);
	~fbScheduler();

	void startup();
	void shutdown();

private:
	bool running;
	fbErrorLogger Error;

	void run();
};

#endif

