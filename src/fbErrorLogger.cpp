/* $Id: fbErrorLogger.cpp,v 1.10 2008/03/20 18:25:25 wyverex Exp $ */

/**
*	fbErrorLogger
*	Error Logger
*	@author Byron Heads
*	@date March 07, 2008
*/

#include "fbErrorLogger.h"
#include "fbLock.h"
#include "fbDate.h"
#include "fbTime.h"

/**
*	fbErrorLogger
*	Stream error Logger
*	@param stream stream for error logger to output too
*	@note Stream copied into out stream
*/
fbErrorLogger::fbErrorLogger(ostream& stream):out(&stream), cs()
{
}

fbErrorLogger::fbErrorLogger(ostream* stream):out(stream), cs()
{
}


/**
*	~fbErrorLogger
*	Destructor
*/
fbErrorLogger::~fbErrorLogger()
{
	if(out) delete out;

	out = NULL;	/// < Null stream pointer
}

/**
*	print
*	Prints a message to the stream
*	@param lvl Level of Error message
*	@param code Current Error Code
*	@param str Additional error string
*	@note This func takes char*
*/
void fbErrorLogger::print(ERROR_LEVEL lvl, ERROR_CODES code, const char* str)
{
	string desc, level, date;

#ifndef Debug
	if(lvl == DEBUG)	/// < if not in debug mode then no debug messages
		return;
#endif
	
	{
		fbLock lock(cs);	/// <lock critical section so text wont write over each other
		errorlevel(lvl, level);		/// < get error level string
		errordesc(code, desc);		/// < get error description
		errordate(date);		/// < get error date and time
		/// print error message
		*out << date.c_str() << '\t' << level.c_str() << '\t' << static_cast<int>(code) << 
			(code == NONE ? "" : "\t") << desc.c_str() << '\t' << str <<  endl;

		//kill on errors
		if(lvl == ERR) exit((int)code);
	}
}

/**
*	print
*	Prints a message to the stream
*	@param lvl Level of Error message
*	@param code Current Error Code
*	@param str Additional error string
*	@note This func takes string
*/
void fbErrorLogger::print(ERROR_LEVEL lvl, ERROR_CODES code, string& str)
{
	string desc, level, date;
	
#ifndef Debug
	if(lvl == DEBUG)	/// < if not in debug mode then no debug messages
		return;
#endif
	
	{
		fbLock lock(cs);	/// <lock critical section so text wont write over each other
		errorlevel(lvl, level);		/// < get error level string
		errordesc(code, desc);		/// < get error description
		errordate(date);		/// < get error date and time		
		/// print error message
		*out << date.c_str() << '\t' << level.c_str() << '\t' << static_cast<int>(code) << 
			(code == NONE ? "" : "\t") << desc.c_str() << '\t' << str.c_str() <<  endl;

		//kill on errors
		if(lvl == ERR) exit((int)code);
	}
}


void fbErrorLogger::errordate(string& date)
{
	fbDate d;
	fbTime t;
	t.hm(date);
	date += '\t';
	d.mdy(date);
}

/**
*	errorlevel
*	Gets the error level as a string
*	@param lvl Level to turn to string
*	@param level String output
*/
void fbErrorLogger::errorlevel(ERROR_LEVEL lvl, string& level)
{
	switch(lvl)
	{
		case(DEBUG):
			level = "Debug";
			break;
		case(INFO):
			level = "Message";
			break;
		case(WARN):
			level = "Warning";
			break;
		case(ERR):
			level = "Error";
			break;
		default:
			level = "UNKNOWN";
	}	
}

/**
*	errordesc
*	Gets the error code as a string
*	@param  code Code to turn into string
*	@param desc Error string output
*/
void fbErrorLogger::errordesc(ERROR_CODES code, string& desc)
{
	switch(code)
	{
		case(OK):
			desc = "SUCCESS";
			break;
		case(NONE):
			desc = "";
			break;
		case(THREADCREATEFAIL):
			desc = "Unable to create a Thread";
			break;
		case(THREADTERMINATEFAILED):
			desc = "Unable to Terminate a Thread";
			break;
		case(THREADSUSPENDFAILED):
			desc = "Unable to Suspend a Thread";
			break;
		case(THREADRESUMEFAILED):
			desc = "Unable to Resume a Thread";
			break;
		case(FAILEDTOOPENDB):
			desc = "Failed to open Database";
			break;
		case(SQLEXECERROR):
			desc = "SQLITE3_EXEC ERROR";
			break;
		default:
			desc = "Unkown Error";
	}
}
