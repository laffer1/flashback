/* $Id: fbScheduler.cpp,v 1.3 2008/03/20 18:46:13 wyverex Exp $ */



#include "fbScheduler.h"


fbScheduler::fbScheduler(fbErrorLogger& errlog):running(false), data(_data)
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

