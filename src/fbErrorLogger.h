/* $Id: fbErrorLogger.h,v 1.6 2008/03/20 18:25:25 wyverex Exp $ */

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
	
private:
	ostream* out;
	fbCriticalSection cs;

	void errordate(string& date);
	static void errorlevel(ERROR_LEVEL lvl, string& level);
	static void errordesc(ERROR_CODES code, string& desc);
};

#endif
