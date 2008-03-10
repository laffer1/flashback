/* $Id: fbDiskDetector.h,v 1.1 2008/03/10 20:08:24 wyverex Exp $ */

#ifndef fbDISKDETECTOR_H
#define fbDISKDETECTOR_H

#include "global.h"
#include "fbThread.h"
#include "fbErrorLogger.h"

/**
*  fbDiskDetector
*  Detects changes in disks or mount points on machine
*/
class fbDiskDetector: public fbThread
{
public:
 	fbDiskDetector(fbErrorLogger& errlog);
	~fbDiskDetector();

	void startup();
	void shutdown();

private:
	bool running;
	fbErrorLogger Error;

	void run();

};

#endif

