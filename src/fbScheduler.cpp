/* $Id: fbScheduler.cpp,v 1.2 2008/03/14 19:18:09 wyverex Exp $ */



#include "fbScheduler.h"


fbScheduler::fbScheduler(fbErrorLogger& errlog):running(false), Error(errlog)
{
}

fbScheduler::~fbScheduler()
{
	if(running)
		shutdown();
}


void fbScheduler::startup()
{
	if(running)
		return;
	start();
}


void fbScheduler::shutdown()
{
	if(!running)
		return;

	stop();
	running = false;
}

void fbScheduler::run()
{
	while(!isStopping())
	{
		_sleep(60*15);
	}
}

