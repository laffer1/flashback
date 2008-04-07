/* $Id: fbData.cpp,v 1.8 2008/04/07 14:00:57 laffer1 Exp $ */

#include "fbData.h"

fbData::fbData():errlog(NULL), db(NULL), config(NULL)
{
	errlog = new fbErrorLogger(new ofstream("/var/log/flashback", ios::out | ios::binary | ios::app));
	debug(NONE, "Error Logger Created");
	config = new fbConfig(errlog);
	debug(NONE, "Config Created");
	db = new fbDatabase(errlog, config->getDBPath());
	debug(NONE, "Database Created");

	debug(NONE, "fbData.this");
}

fbData::~fbData()
{
	if(errlog) delete errlog;
	if(db) delete db;
	if(config) delete config;

	debug(NONE, "fbData.~this");
	errlog = NULL;
	db = NULL;
	config = NULL;
}


//fbErrorLogger Functions
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
	errlog->print(DBUG, code, str);
}
void fbData::debug(ERROR_CODES code, string& str)
{
	errlog->print(DBUG, code, str);
}


//fbConfig functions
int fbData::loadConfig()
{
    return config->load();
}

int fbData::saveConfig()
{
    return config->save();
}

void fbData::setWebServerAddr(const string& strAddr)
{
	config->setWebServerAddr(strAddr);
}

void fbData::setWebServerPort(const int port)
{
	config->setWebServerPort(port);
}

void fbData::setWebServerRootPath(const string& strPath)
{
	config->setWebServerRootPath(strPath);
}

void fbData::setDBPath(const string& strPath)
{
	config->setDBPath(strPath);
}

const string& fbData::getWebServerAddr()
{
	return config->getWebServerAddr();
}

const int   fbData::getWebServerPort()
{
	return config->getWebServerPort();
}

const string& fbData::getWebServerRootPath()
{
	return config->getWebServerRootPath();
}

const string& fbData::getDBPath()
{
	return config->getDBPath();
}
