/* $Id: fbData.cpp,v 1.5 2008/03/22 20:30:12 wyverex Exp $ */

#include "fbData.h"

fbData::fbData():errlog(NULL), db(NULL), config(NULL)
{
	errlog = new fbErrorLogger(new ofstream("/var/log/flashback", ios::out | ios::binary | ios::app));
	config = new fbConfig(errlog);
	db = new fbDatabase(errlog, config->getDBPath());
}

fbData::~fbData()
{
	if(errlog) delete errlog;
	if(db) delete db;
	if(config) delete config;

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
void fbData::setWebServerAddr(const char* strAddr)
{
	config->setWebServerAddr(strAddr);
}

void fbData::setWebServerPort(const char* strPort)
{
	config->setWebServerPort(strPort);
}

void fbData::setWebServerPort(const int port)
{
	config->setWebServerPort(port);
}

void fbData::setWebServerRootPath(const char* strPath)
{
	config->setWebServerRootPath(strPath);
}

void fbData::setDBPath(const char* strPath)
{
	config->setDBPath(strPath);
}

const char* fbData::getWebServerAddr()
{
	return config->getWebServerAddr();
}

const int   fbData::getWebServerPort()
{
	return config->getWebServerPort();
}

const char* fbData::setWebServerRootPath()
{
	return config->setWebServerRootPath();
}

const char* fbData::getDBPath()
{
	return config->getDBPath();
}







