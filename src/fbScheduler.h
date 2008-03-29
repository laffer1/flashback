/* $Id: fbScheduler.h,v 1.4 2008/03/29 22:19:29 wyverex Exp $ */

#ifndef fbSCHEDULER_H
#define fbSCHEDULER_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"

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
	fbScheduler(fbData* _data);
	~fbScheduler();

	void startup();
	void shutdown();

private:
	fbData* data;
	void run();
};

#endif

