/* $Id: fbCore.cpp,v 1.10 2008/03/31 01:26:04 ctubbsii Exp $ */

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

bool FlashBackRunning;
void sigterm_handler(int s);

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

	//global running varible
	FlashBackRunning = true;

	// Shutdown Detector
	signal(SIGTERM, sigterm_handler);

	//core loop
	while(FlashBackRunning)
	{
		//what was the plan for here???
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

void sigterm_handler(int s)
{
    signal(SIGTERM, sigterm_handler);
    FlashBackRunning = false;
}
