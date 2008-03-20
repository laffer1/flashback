/* $Id */

/**
*  fbDiskDetector.cpp
*  Detects changesin disk or mount points
*  @author Byron Heads
*  @date March 10, 2008
*/


#include "fbDiskDetector.h"


fbDiskDetector::fbDiskDetector(fbData* _data):fbThread(_data), running(false), data(_data)
{
}

fbDiskDetector::~fbDiskDetector()
{
	if(running)
		stop();
}

void fbDiskDetector::startup()
{
	if(running)
		return;

	running = true;
	data->msg(NONE, "Diskdetector Started.");
	start();
}

void fbDiskDetector::shutdown()
{
	if(!running)
		return;

	stop();
		data->msg(NONE, "Diskdetector Stopped.");
	running = false;
}

void fbDiskDetector::run()
{
	while(!isStopping())
		_sleep(60);
}

