/* $Id: fbScheduler.cpp,v 1.6 2008/04/07 13:20:47 wyverex Exp $ */



#include "fbScheduler.h"


fbScheduler::fbScheduler(fbData* _data):fbThread(_data), data(_data)
{
	data->debug(NONE, "fbScheduler.this");
}

fbScheduler::~fbScheduler()
{
	data->debug(NONE, "fbScheduler.~this");
	shutdown();
}


void fbScheduler::startup()
{
	start();
}


void fbScheduler::shutdown()
{
	stop();
}

void fbScheduler::run()
{
	data->debug(NONE, "fbScheduler.run");
	string desc("test");
	fbDate date;
	fbTime time;
	string path("/var/log");

	//data->db->addBackupJob(desc,date,time,path);

	while(!isStopping())
	{
		//Backup querry
		_sleep(60*15);
	}
}


