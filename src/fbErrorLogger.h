/* $Id: fbErrorLogger.h,v 1.7 2008/03/22 20:19:44 wyverex Exp $ */

#ifndef fbERRORLOGGER_H
#define fbERRORLOGGER_H

#include "global.h"
#include "fbErrorLevel.h"
#include "fbErrorCodes.h"
#include "fbCriticalSection.h"


/**
*	fbErrorLogger
*	Error Logger
*	@author Byron Heads
*	@date March 07, 2008
*	@note 3/7/08 - Created fbErrorLogger
*/
class fbErrorLogger
{
public:
	fbErrorLogger(ostream& stream);
	fbErrorLogger(ostream* stream);
	~fbErrorLogger();

	void print(ERROR_LEVEL lvl, ERROR_CODES code, const char* str);
	void print(ERROR_LEVEL lvl, ERROR_CODES code, string& str);

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
	ostream* out;
	fbCriticalSection cs;

	void errordate(string& date);
	static void errorlevel(ERROR_LEVEL lvl, string& level);
	static void errordesc(ERROR_CODES code, string& desc);
};

#endif
