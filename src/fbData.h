/* $Id: fbData.h,v 1.5 2008/03/23 01:40:45 ctubbsii Exp $ */



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
	void err(ERROR_CODES code, const char* str);
	void err(ERROR_CODES code, string& str);
	void warn(ERROR_CODES code, const char* str);
	void warn(ERROR_CODES code, string& str);
	void msg(ERROR_CODES code, const char* str);
	void msg(ERROR_CODES code, string& str);
	void debug(ERROR_CODES code, const char* str);
	void debug(ERROR_CODES code, string& str);

	//config functions

	int loadConfig();
	int saveConfig();

	void setWebServerAddr(const string& strAddr);
	void setWebServerPort(const int port);
	void setWebServerRootPath(const string& strPath);
	void setDBPath(const string& strPath);
	const string& getWebServerAddr();
	const int   getWebServerPort();
	const string& setWebServerRootPath();
	const string& getDBPath();


private:
	fbErrorLogger* errlog;
	fbDatabase*    db;
	fbConfig*    config;

};
#endif

