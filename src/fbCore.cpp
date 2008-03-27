/* $Id: fbCore.cpp,v 1.8 2008/03/27 17:48:14 wyverex Exp $ */

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
	data.debug(NONE, "Making Scheduler");  //should only show in debug mode!	
	fbScheduler scheduler(&data);
	scheduler.startup();
	data.debug(NONE, "Scheduler Made");  //should only show in debug mode!

	//DiskDetector
	//fbDiskDetector diskdetect(&data);
	//diskdetect.startup();

	data.debug(NONE, "Making Web Server");  //should only show in debug mode!
	fbHttpServer http(&data);
	http.startup();
	data.debug(NONE, "Web Server Running");  //should only show in debug mode!


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
	data.debug(NONE, "Shutting Down Web Server");  //should only show in debug mode!
        http.shutdown();


	//scheduler
	data.debug(NONE, "Shutting Down Scheduler");  //should only show in debug mode!
	scheduler.shutdown();

	//diskdetector
	//skdetect.shutdown();

	data.debug(NONE, "Flashback Exiting");  //should only show in debug mode!
}

