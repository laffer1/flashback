/* $Id */

/**
*  fbDiskDetector.cpp
*  Detects changesin disk or mount points
*  @author Byron Heads
*  @date March 10, 2008
*/


#include "fbDiskDetector.h"


fbDiskDetector::fbDiskDetector(fbErrorLogger& errlog):running(false), Error(errlog)
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
	start();
}

void fbDiskDetector::shutdown()
{
	if(!running)
		return;

	stop();
	running = false;
}

void fbDiskDetector::run()
{
	while(!isStopping())
		_sleep(60*5);
}

