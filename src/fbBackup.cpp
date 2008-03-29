

#include "fbBackup.h"
fbBackup::fbBackup(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), from(src), to(dest)
{
	data->debug(NONE,"fbData.this");
	startDelete();
}

fbBackup::~fbBackup()
{
	data->debug(NONE,"fbData.~this");
}


void fbBackup::run()
{
	string cmd = "tar -cf ";
	cmd += to;
	cmd += " ";
	cmd += to;

	system(cmd.c_str());
}

