/* $Id: fbCore.cpp,v 1.6 2008/03/21 00:52:22 laffer1 Exp $ */

/**
*  fbCore.cpp
*  @author Byron Heads
*  @date March 10, 2008	
*/



#include "global.h"
#include "fbData.h"
#include "fbDiskDetector.h"
#include "fbScheduler.h"
#include "fbHttpServer.h"

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
	fbScheduler scheduler(&data);
	scheduler.startup();

	//DiskDetector
	fbDiskDetector diskdetect(&data);
	diskdetect.startup();

	fbHttpServer http(&data);
	http.startup();


	// Shutdown Detector
	/*while(1)
	{
#ifdef Win32
		Sleep(1000 * 60);
#else
		sleep(60);
#endif
	}*/
	

	//shutdown 

        http.shutdown();

	//scheduler
	scheduler.shutdown();

	//diskdetector
	diskdetect.shutdown();	
}

