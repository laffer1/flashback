/* $Id: fbCore.cpp,v 1.20 2008/04/23 01:21:42 ctubbsii Exp $ */

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

volatile bool FlashBackRunning;
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

	//data.debug(NONE, "Testing %s %d %f", "1",2,3.0f);
	//data.msg(NONE, "Testing %s %d %f", "1",2,3.0f);
	//data.warn(NONE, "Testing %s %d %f", "1",2,3.0f);
	//data.err(NONE, "Testing %s %d %f", "1",2,3.0f);

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

	// data.addBackupJob(new string("backup test"), new fbDate, new fbTime, new string("/var/log/"));

	//data.addRestoreJob(new string("something0.tar"), new string("/home/backups/something/"));



	//core loop
	while(FlashBackRunning)
	{
		//check for exit (every 10 seconds)
#ifdef Win32
		Sleep(1000 * 10);
#else
		sleep(10);
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
