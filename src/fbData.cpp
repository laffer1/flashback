/* $Id: fbData.cpp,v 1.12 2008/04/10 18:33:36 wyverex Exp $ */

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
/*void fbData::err(ERROR_CODES code, const char* str)
{
	errlog->print(ERR, code, str);
}*/
void fbData::err(ERROR_CODES code, string& str)
{
	errlog->print(ERR, code, str);
}

/*void fbData::warn(ERROR_CODES code, const char* str)
{
	errlog->print(WARN, code, str);
}*/
void fbData::warn(ERROR_CODES code, string& str)
{
	errlog->print(WARN, code, str);
}

/*void fbData::msg(ERROR_CODES code, const char* str)
{
	errlog->print(INFO, code, str);
}*/
void fbData::msg(ERROR_CODES code, string& str)
{
	errlog->print(INFO, code, str);
}

/*void fbData::debug(ERROR_CODES code, const char* str)
{
	errlog->print(DBUG, code, str);
}*/
void fbData::debug(ERROR_CODES code, string& str)
{
	errlog->print(DBUG, code, str);
}



void fbData::err(ERROR_CODES code, const char* str,  ...)
{
	va_list list;
	char buff[500];
	string mesg;
	va_start(list, str);
	vsprintf(buff, str, list);
	mesg = buff;
	err(code, mesg);
	va_end(list);
}


void fbData::warn(ERROR_CODES code, const char* str, ...)
{
	va_list list;
	char buff[500];
	string mesg;
	va_start(list, str);
	vsprintf(buff, str, list);
	mesg = buff;
	warn(code, mesg);
	va_end(list);
}

void fbData::msg(ERROR_CODES code, const char* str,  ...)
{
	va_list list;
	char buff[500];
	string mesg;
	va_start(list, str);
	vsprintf(buff, str, list);
	mesg = buff;
	msg(code, mesg);
	va_end(list);
}


void fbData::debug(ERROR_CODES code, const char* str,...)
{
	va_list list;
	char buff[500];
	string mesg;
	va_start(list, str);
	vsprintf(buff, str, list);
	mesg = buff;
	debug(code, mesg);
	va_end(list);
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



// Database functions
// addBackupJob(new string("test"), new fbDate, new fbTime, new string("/var/log"));
//
bool fbData::addBackupJob(string* desc, fbDate* date, fbTime* time, string* path, Repeat_type rt, int rv)
{
	bool ret = db->addBackupJob(*desc, *date, *time, *path, rt, rv);
	delete desc;
	delete date;
	delete time;
	delete path;
	return ret;
}


bool fbData::addRepo(string* desc, fbDate* date, fbTime* time, string* path, string* tarfile)
{
	bool ret = db->addRepo(*desc, *date, *time, *path, *tarfile);
	delete desc;
	delete date;
	delete time;
	delete path;
	delete tarfile;
	return ret;
}


bool fbData::querryBackups()
{
	return db->querryBackups();

}

bool fbData::addRestoreJob(string* tarfile, string* dest)
{
	bool ret = db->addRestoreJob(*tarfile, *dest);
	delete tarfile;
	delete dest;
	return ret;
}

bool fbData::querryRestore()
{
	return db->querryRestore();
}

bool fbData::querryRepo()
{
	return db->querryRepo();
}



