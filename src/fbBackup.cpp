/* $Id: fbBackup.cpp,v 1.2 2008/04/03 07:56:43 ctubbsii Exp $ */

#include "fbBackup.h"

fbBackup::fbBackup(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), from(src), to(dest)
{
	data->debug(NONE,"fbBackup.this");
	startDelete();
}

fbBackup::~fbBackup()
{
	data->debug(NONE,"fbBackup.~this");
}


void fbBackup::run()
{
    /* libarchive backup code here */

	string cmd = "tar -cf ";
	cmd += to;
	cmd += " ";
	cmd += to;

	system(cmd.c_str());
}
