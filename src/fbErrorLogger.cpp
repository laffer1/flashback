/* $Id: fbErrorLogger.cpp,v 1.20 2008/04/20 15:29:11 wyverex Exp $ */

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
*	@param stream stream for error logger to output too, ref
*	@note Stream copied into out stream
*/
fbErrorLogger::fbErrorLogger(ostream& stream):out(&stream), cs()
{
	*out << endl;
	debug(NONE, "fbErrorLogger.this");
}

/**
*	fbErrorLogger
*	Stream error Logger
*	@param stream stream for error logger to output too, pointer
*	@note Stream copied into out stream
*/
fbErrorLogger::fbErrorLogger(ostream* stream):out(stream), cs()
{
	*out << endl;
	debug(NONE, "fbErrorLogger.this");
}


/**
*	~fbErrorLogger
*	Destructor
*/
fbErrorLogger::~fbErrorLogger()
{
	debug(NONE, "fbErrorLogger.~this");
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
	if(lvl == DBUG)	/// < if not in debug mode then no debug messages
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
			
		// ?? since when does endl not flush the stream?
		out->flush(); 
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
	if(lvl == DBUG)	/// < if not in debug mode then no debug messages
		return;
#endif

	{
		fbLock lock(cs);	/// <lock critical section so text wont write over each other
		errorlevel(lvl, level);		/// < get error level string
		errordesc(code, desc);		/// < get error description
		errordate(date);		/// < get error date and time
		/// print error message
		*out << date << '\t' << level << '\t' << static_cast<int>(code) <<
			(code == NONE ? "" : "\t") << desc << '\t' << str <<  endl;

		//forgot flush here, though I thought endl flushs....
		out->flush(); 
			
		//kill on errors
		if(lvl == ERR) exit((int)code);
	}
}

/**
*	errordate
*	Appends the date and time to input string
*	@param date String refrence to append date/time to
*	@note hour:min	m/d/y
*/
void fbErrorLogger::errordate(string& date)
{
	fbDate d;
	fbTime t;
	t.hm(date);
	date += '\t';
	d.mdy(date);
}


/**  
*	@note The auto error code generaton stuff has basiclly been abandoned at this point
*		I had planned to modifiy this to load Error Strings from a file so we could do more
*		the just english... not enough time...
*/

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
		case(DBUG):
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
		case(CONFIGFAILEDTOLOAD):
			desc = "Failed to load config file";
			break;
		case (CONFIGUNKOWNVALUE):
			desc = "Unknown config settings";
			break;
		case(CONFIGINCOMPLETE):
			desc = "Incomplete Config File";
			break;
		case(SQLEXECERROR):
			desc = "SQLITE3_EXEC ERROR";
			break;
		default:
			desc = "Unkown Error";
	}
}

/**
*	err
*	Sends an error message to the Log
*	@param code Pre written Error Codes
*	@param str Error Message string
*	@param ... variable param list, alows to build error message like printf
*/
void fbErrorLogger::err(ERROR_CODES code, const char* str, ...)
{
	va_list list;  
	char buff[1024*4];  //maybe make the buffer size a const...?
	string mesg;

	va_start(list, str);  //build the list
	vsnprintf(buff, sizeof(buff)-1, str, list);  //build the string
	va_end(list);

	print(ERR, code, buff); //pass it on to print
}

/**
*	err
*	String based error function, can't build complex messages like previos function	
*/
void fbErrorLogger::err(ERROR_CODES code, string& str)
{
	print(ERR, code, str);
}

/**
*	warn
*	Sends an warnings to the Log
*	@param code Pre written Error Codes
*	@param str Error Message string
*	@param ... variable param list, alows to build error message like printf
*/
void fbErrorLogger::warn(ERROR_CODES code, const char* str, ...)
{
	va_list list;
	char buff[1024*4];
	string mesg;

	va_start(list, str);
	vsnprintf(buff, sizeof(buff)-1, str, list);
	va_end(list);

	print(WARN, code, buff);
}

/**
*	warn
*	String based error function, can't build complex messages like previos function	
*/
void fbErrorLogger::warn(ERROR_CODES code, string& str)
{
	print(WARN, code, str);
}

/**
*	err
*	Sends an message to the Log
*	@param code Pre written Error Codes, prolly doesn't need
*	@param str Error Message string
*	@param ... variable param list, alows to build error message like printf
*/
void fbErrorLogger::msg(ERROR_CODES code, const char* str, ...)
{
	va_list list;
	char buff[1024*4];
	string mesg;

	va_start(list, str);
	vsnprintf(buff, sizeof(buff)-1, str, list);
	va_end(list);

	print(INFO, code, buff);
}

/**
*	msg
*	String based error function, can't build complex messages like previos function	
*/
void fbErrorLogger::msg(ERROR_CODES code, string& str)
{
	print(INFO, code, str);
}

/**
*	err
*	Sends an debug message to the Log
*	@param code Pre written Error Codes, prolly doesn't need
*	@param str Error Message string
*	@param ... variable param list, alows to build error message like printf
*/
void fbErrorLogger::debug(ERROR_CODES code, const char* str, ...)
{
	va_list list;
	char buff[1024*4];
	string mesg;

	va_start(list, str);
	vsnprintf(buff, sizeof(buff)-1, str, list);
	va_end(list);

	print(DBUG, code, buff);
}

/**
*	debug
*	String based error function, can't build complex messages like previos function	
*/
void fbErrorLogger::debug(ERROR_CODES code, string& str)
{
	print(DBUG, code, str);
}
