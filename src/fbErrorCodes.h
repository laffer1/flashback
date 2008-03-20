/* $Id: fbErrorCodes.h,v 1.7 2008/03/20 18:46:13 wyverex Exp $ */

#ifndef fbERRORCODES_H
#define fbERRORCODES_H

/**
*	ERROR_CODES
*	Error level enumeration for error log
*	@author Byron Heads
*	@date 5/07/2008
*/
enum ERROR_CODES
{
	UNKNOWN = -1,
	OK = 0,
	NONE,
	THREADCREATEFAIL,
	THREADTERMINATEFAILED,
	THREADSUSPENDFAILED,
	THREADRESUMEFAILED,

	FAILEDTOOPENDB,
	SQLEXECERROR
	
	
};

#endif
