/* $Id: fbErrorLogger.cpp,v 1.2 2008/03/08 05:29:28 laffer1 Exp $ */

/**
*	fbErrorLogger
*	Error Logger
*	@author Byron Heads
*	@date March 07, 2008
*/

#include "fbErrorLogger.h"
#include "fbCriticalSection.h"

fbErrorLogger::fbErrorLogger(ostream& stream):out(&stream)
{
}

fbErrorLogger::~fbErrorLogger()
{
}

void fbErrorLogger::print(ERROR_LEVEL lvl, ERROR_CODES code, char* str)
{
	static fbCriticalSection cs;
	string desc, level;
	
	cs.lock();
	errorlevel(lvl, level);
	errordesc(code, desc);
	*out << level.c_str() << " " << static_cast<int>(code) << ": " 
		 << desc.c_str() << " - " << str <<  endl;
	cs.unlock();
}

void fbErrorLogger::print(ERROR_LEVEL lvl, ERROR_CODES code, string& str)
{
	static fbCriticalSection cs;
	string desc, level;
	
	cs.lock();
	errorlevel(lvl, level);
	errordesc(code, desc);
	*out << level.c_str() << " " << static_cast<int>(code) << ": " 
		 << desc.c_str() << " - " << str.c_str() <<  endl;
	cs.unlock();
}

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
		default:
			desc = "Unkown Error";
	}
}
