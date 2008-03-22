/* $Id: fbData.h,v 1.4 2008/03/22 20:30:12 wyverex Exp $ */



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
	void setWebServerAddr(const char* strAddr);
	void setWebServerPort(const char* strPort);
	void setWebServerPort(const int port);
	void setWebServerRootPath(const char* strPath);
	void setDBPath(const char* strPath);
	const char* getWebServerAddr();
	const int   getWebServerPort();
	const char* setWebServerRootPath();
	const char* getDBPath();


private:
	fbErrorLogger* errlog;
	fbDatabase*    db;
	fbConfig*    config;

};
#endif

