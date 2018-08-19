/* $Id: fbDiskDetector.h,v 1.2 2008/03/20 18:25:25 wyverex Exp $ */

#ifndef fbDISKDETECTOR_H
#define fbDISKDETECTOR_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"

/**
*  fbDiskDetector
*  Detects changes in disks or mount points on machine
*/
class fbDiskDetector: public fbThread
{
public:
 	fbDiskDetector(fbData* _data);
	~fbDiskDetector();

	void startup();
	void shutdown();

private:
	bool running;
	fbData* data;

	void run();

};

#endif

