/* $Id: fbCore.cpp,v 1.3 2008/03/10 20:26:26 wyverex Exp $ */

/**
*  fbCore.cpp
*  @author Byron Heads
*  @date March 10, 2008	
*/



#include "global.h"
#include "fbErrorLogger.h"
#include "fbDiskDetector.h"
#include "fbScheduler.h"

/**
* core
*  Flashbacks core running file, main calls core
*  @note This is another usable Thread
*/
void core()
{
	//Local Settings Object

	fbErrorLogger Error(new ofstream("temp.log"));
		






	//Scheduler
	fbScheduler scheduler(Error);
	scheduler.startup();

	//DiskDetector
	fbDiskDetector diskdetect(Error);
	diskdetect.startup();


	// Shutdown Detector
	while(1)
	{
#ifdef Win32
		Sleep(1000 * 60);
#else
		sleep(60);
#endif
	}
	

	//shutdown 



	//scheduler
	scheduler.shutdown();

	//diskdetector
	diskdetect.shutdown();	
}

