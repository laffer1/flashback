/* $Id: fbScheduler.h,v 1.1 2008/03/10 20:26:26 wyverex Exp $ */

#ifndef fbSCHEDULER_H
#define fbSCHEDULER_H

#include "global.h"
#include "fbThread.h"
#include "fbErrorLogger.h"

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

