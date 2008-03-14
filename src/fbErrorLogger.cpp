/* $Id: fbErrorLogger.cpp,v 1.8 2008/03/14 20:11:36 wyverex Exp $ */

/**
*	fbErrorLogger
*	Error Logger
*	@author Byron Heads
*	@date March 07, 2008
*/

#include "fbErrorLogger.h"
#include "fbLock.h"

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
void fbErrorLogger::print(ERROR_LEVEL lvl, ERROR_CODES code, char* str)
{
	string desc, level;

#ifndef Debug
	if(lvl == DEBUG)	/// < if not in debug mode then no debug messages
		return;
#endif
	
	{
		fbLock lock(cs);	/// <lock critical section so text wont write over each other
		errorlevel(lvl, level);		/// < get error level string
		errordesc(code, desc);		/// < get error description
		/// print error message
		*out << level.c_str() << " " << static_cast<int>(code) << ": " 
			 << desc.c_str() << "  " << str <<  endl;
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
	string desc, level;
	
#ifndef Debug
	if(lvl == DEBUG)	/// < if not in debug mode then no debug messages
		return;
#endif
	
	{
		fbLock lock(cs);	/// <lock critical section so text wont write over each other
		errorlevel(lvl, level);		/// < get error level string
		errordesc(code, desc);		/// < get error description
		/// print error message
		*out << level.c_str() << " " << static_cast<int>(code) << ": " 
			<< desc.c_str() << " - " << str.c_str() <<  endl;
	}
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
			level = "DEBUG";
			break;
		case(INFO):
			level = "MSG";
			break;
		case(WARN):
			level = "WARNING";
			break;
		case(ERR):
			level = "ERROR";
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
