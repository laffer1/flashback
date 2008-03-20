/* $Id: fbCore.cpp,v 1.4 2008/03/20 18:25:25 wyverex Exp $ */

/**
*  fbCore.cpp
*  @author Byron Heads
*  @date March 10, 2008	
*/



#include "global.h"
#include "fbData.h"
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
	fbData data;
	
	data.debug(NONE, "Running Debug Mode");  //should only show in debug mode!
	data.msg(NONE, "Flashback Started");
	

	//Scheduler
	//fbScheduler scheduler(Error);
	//scheduler.startup();

	//DiskDetector
	fbDiskDetector diskdetect(&data);
	diskdetect.startup();


	// Shutdown Detector
	//while(1)
	{
#ifdef Win32
		Sleep(1000 * 60);
#else
		sleep(60);
#endif
	}
	

	//shutdown 



	//scheduler
	//scheduler.shutdown();

	//diskdetector
	diskdetect.shutdown();	
}

