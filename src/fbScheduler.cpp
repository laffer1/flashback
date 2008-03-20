/* $Id: fbScheduler.cpp,v 1.4 2008/03/20 19:07:49 wyverex Exp $ */



#include "fbScheduler.h"


fbScheduler::fbScheduler(fbData* _data):fbThread(_data), running(false), data(_data)
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

