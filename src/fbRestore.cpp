/* $Id: fbRestore.cpp,v 1.1 2008/04/03 07:56:43 ctubbsii Exp $ */

#include "fbRestore.h"

fbRestore::fbRestore(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), from(src), to(dest)
{
	data->debug(NONE,"fbRestore.this");
	startDelete();
}

fbRestore::~fbRestore()
{
	data->debug(NONE,"fbRestore.~this");
}


void fbRestore::run()
{
    /* libarchive restore code here */
}
