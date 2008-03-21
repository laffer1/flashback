/* $Id: fbData.h,v 1.2 2008/03/21 00:35:59 wyverex Exp $ */



#ifndef fbDATA_H
#define fbDATA_H

#include "global.h"
#include "fbErrorLogger.h"
#include "fbDatabase.h"
//#include "fbConfig.h"

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

private:
	fbErrorLogger* errlog;
	fbDatabase*    db;
	//fbConfig*    config;

};
#endif

