/* $Id: fbScheduler.cpp,v 1.7 2008/04/08 19:16:46 wyverex Exp $ */



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
	string desc, path;
	fbDate date;
	fbTime time;
	int repeatval, index;
	Repeat_type repeat;
	bool ret;


	while(!isStopping())
	{
		//Backup querry
		data->querryBackups();

		do
		{	
			ret = data->db->getBackupRow(desc, date, time, path, &repeat, 
				&repeatval, &index);
			if(ret)
			{
				//backup!?!
				char buff[500];
				string msg;
				sprintf(buff, "Backing up: %s: %s at %ld %ld %d %d",
				desc.c_str(), path.c_str(), time.getTicks(), date.getJulian(),
				repeat, repeatval);
				msg = buff;
				data->debug(NONE, msg);

				//test repeat!
			}
		}while(ret);
		



		//sleep 15 mins! 60 * 15
		_sleep(10);
	}
}


