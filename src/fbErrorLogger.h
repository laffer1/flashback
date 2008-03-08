
#ifndef fbERRORLOGGER_H
#define fbERRORLOGGER_H

#include "global.h"

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
	~fbErrorLogger();

	void print(ERROR_LEVEL lvl, ERROR_CODES code, char* str);
	void print(ERROR_LEVEL lvl, ERROR_CODES code, string& str);
	
private:
	ostream* out;
	
	void errorlevel(ERROR_LEVEL lvl, string& level);
	void errordesc(ERROR_CODES code, string& desc);
};

#endif
