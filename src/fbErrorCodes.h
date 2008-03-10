/* $Id: fbErrorCodes.h,v 1.4 2008/03/10 19:01:54 wyverex Exp $ */

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
	NONE = 1,

	THREADCREATEFAIL = 2,
	THREADTERMINATEFAILED = 3,
	THREADSUSPENDFAILED = 4,
	THREADRESUMEFAILED = 5,


	OTHER = 1000	
};

#endif
