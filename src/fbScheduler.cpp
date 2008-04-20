/* $Id: fbScheduler.cpp,v 1.18 2008/04/20 15:36:48 wyverex Exp $ */



#include "fbScheduler.h"
#define bk_path "/var/flashback/" ///< set path to backup location... this should be comming from config...


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
		//Backup query
		data->queryBackups();
		do
		{
			ret = data->db->getBackupRow(desc, date, time, path, &repeat,
				&repeatval, &index);
			if(ret)
			{
				//re sync time/date
				time.setTicksLocal();
				date.setJulianLocal();

				//backup!?!
				data->debug(NONE, "Backing up: %d %s: %s at %ld %ld %d %d", index,
				desc.c_str(), path.c_str(), time.getTicks(), date.getJulian(),
				repeat, repeatval);

				//do the backup here!
				char buff[1024*4];
				snprintf(buff, sizeof(buff)-1, "%s%ld%ld%d.tar", bk_path, date.getJulian(), time.getTicks(), index);
				string msg = buff;
				data->db->addRepo(desc, date, time, path, msg);
				new fbBackup(data, path, msg);


				//test repeat!
				data->db->deleteRow("backup", index);
				switch(repeat)
				{
					case MINS:
						time.addMin(repeatval);
						//need to check for day overflow!
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					case HOUR:
						time.addHour(repeatval);
						//need to check for day overflow
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					case DAY:
						date.addDay(repeatval);
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					case WEEK:
						date.addWeek(repeatval);
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					case MONTH:
						date.addMonth(repeatval);
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					case YEAR:
						date.addMonth(repeatval*12);
						data->db->addBackupJob(desc, date, time, path, repeat, repeatval);
						break;
					default:
						break;
				}

			}
		}while(ret);

		//check restore here... mostly the same.. but no repeats

		//restore query
		data->queryRestore();
		do
		{
			ret = data->db->getRestoreRow(desc, path, &index);
			if(ret)
			{
				data->msg(NONE, "Restoring: %d %s: %s", index, desc.c_str(), path.c_str());

				//do the restore here!
				//			                   tar file,   path
				new fbRestore(data, desc, path);
	
				//remove row
				data->db->deleteRow("restore", index);

			}
		}while(ret);

		//sleep 15 mins! 60 * 15
		_sleep(60);
	}
}
