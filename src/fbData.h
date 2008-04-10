/* $Id: fbData.h,v 1.11 2008/04/10 18:33:36 wyverex Exp $ */



#ifndef fbDATA_H
#define fbDATA_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbDatabase.h"
#include "fbConfig.h"

class fbData
{
public:
	fbData();
	~fbData();

	//ErrorLogger Functions
	//void err(ERROR_CODES code, const char* str);
	void err(ERROR_CODES code, string& str);
	//void warn(ERROR_CODES code, const char* str);
	void warn(ERROR_CODES code, string& str);
	//void msg(ERROR_CODES code, const char* str);
	void msg(ERROR_CODES code, string& str);
	//void debug(ERROR_CODES code, const char* str);
	void debug(ERROR_CODES code, string& str);


	void err(ERROR_CODES code, const char* str,  ...);
	void warn(ERROR_CODES code, const char* str, ...);
	void msg(ERROR_CODES code, const char* str,  ...);
	void debug(ERROR_CODES code, const char* str,...);


	//config functions

	int loadConfig();
	int saveConfig();

	void setWebServerAddr(const string& strAddr);
	void setWebServerPort(const int port);
	void setWebServerRootPath(const string& strPath);
	void setDBPath(const string& strPath);
	const string& getWebServerAddr();
	const int   getWebServerPort();
	const string& getWebServerRootPath();
	const string& getDBPath();

	//Database functions
	bool addBackupJob(string* desc, fbDate* date, fbTime* time, string* path, Repeat_type rt = ONCE, int rv = 0);
	bool querryBackups();
	bool addRestoreJob(string* tarfile, string* dest);
	bool querryRestore();
	bool addRepo(string* desc, fbDate* date, fbTime* time, string* path, string* tarfile);
	bool querryRepo();

	
//private:
	fbErrorLogger* errlog;
	fbDatabase*    db;
	fbConfig*    config;

};
#endif

