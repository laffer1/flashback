/* $Id: fbScheduler.h,v 1.6 2008/04/20 15:36:48 wyverex Exp $ */

#ifndef fbSCHEDULER_H
#define fbSCHEDULER_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include "fbBackup.h"
#include "fbRestore.h"

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
	fbScheduler(fbData* _data); ///< creates this
	~fbScheduler();	

	void startup();  ///< starts this thread
	void shutdown(); ///< stops this thread

private:
	fbData* data;  ///< access to logger, SQL, and config
	void run();  ///< thread run function
};
#endif
