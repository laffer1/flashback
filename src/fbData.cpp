/* $Id: fbData.cpp,v 1.1 2008/03/21 00:28:17 wyverex Exp $ */

#include "fbData.h"

fbData::fbData():errlog(NULL), db(NULL)/*, config(NULL)*/
{
	errlog = new fbErrorLogger(new ofstream("/var/log/flashback", ios::out | ios::binary | ios::app));
	//config = new fbConfig(..);
	//db = new fbDatabase(config->get_s(DB));
}

fbData::~fbData()
{
	if(errlog) delete errlog;
	if(db) delete db;
	//if(config) delete config

	errlog = NULL;
	db = NULL;
	//config = NULL;
}



void fbData::err(ERROR_CODES code, const char* str)
{
	errlog->print(ERR, code, str);
}
void fbData::err(ERROR_CODES code, string& str)
{
	errlog->print(ERR, code, str);
}

void fbData::warn(ERROR_CODES code, const char* str)
{
	errlog->print(WARN, code, str);
}
void fbData::warn(ERROR_CODES code, string& str)
{
	errlog->print(WARN, code, str);
}

void fbData::msg(ERROR_CODES code, const char* str)
{
	errlog->print(INFO, code, str);
}
void fbData::msg(ERROR_CODES code, string& str)
{
	errlog->print(INFO, code, str);
}

void fbData::debug(ERROR_CODES code, const char* str)
{
	errlog->print(DEBUG, code, str);
}
void fbData::debug(ERROR_CODES code, string& str)
{
	errlog->print(DEBUG, code, str);
}
