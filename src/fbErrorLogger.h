/* $Id: fbErrorLogger.h,v 1.9 2008/04/20 15:29:11 wyverex Exp $ */

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
	fbErrorLogger(ostream& stream);	///< Refrence
	fbErrorLogger(ostream* stream);	/// < Pointer
	~fbErrorLogger();	

	/// Print via char  string
	void print(ERROR_LEVEL lvl, ERROR_CODES code, const char* str);
	/// Print via c++ string
	void print(ERROR_LEVEL lvl, ERROR_CODES code, string& str);

	//ErrorLogger Functions
	
	///Prints Errors to log, Errors kill the system
	void err(ERROR_CODES code, const char* str, ...);
	void err(ERROR_CODES code, string& str);
	
	/// Prints warnings
	void warn(ERROR_CODES code, const char* str, ...);
	void warn(ERROR_CODES code, string& str);
	
	/// Prints messages
	void msg(ERROR_CODES code, const char* str, ...);
	void msg(ERROR_CODES code, string& str);
	
	/// Prints Debug Information
	void debug(ERROR_CODES code, const char* str, ...);
	void debug(ERROR_CODES code, string& str);
	
private:
	ostream* out;	/// < file stream to log
	fbCriticalSection cs;  ///< lock on writing to the log

	void errordate(string& date);	///< adds date time to a string
	
	/// Remains of error string generation...
	static void errorlevel(ERROR_LEVEL lvl, string& level);
	static void errordesc(ERROR_CODES code, string& desc);
};

#endif
