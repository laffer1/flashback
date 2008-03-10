/* $Id: fbCore.cpp,v 1.1 2008/03/10 19:35:30 wyverex Exp $ */

/**
*
*
*/



#include "global.h"
#include "fbErrorLogger.h"

void core()
{
	//Local Settings Object

	fbErrorLogger Error(new ofstream("temp.log"));
		

	// Shutdown Detector
	while(1)
	{
#ifdef Win32
		Sleep(1000 * 60);
#else
		sleep(60);
#endif
	}
	

	//shutdown 
	
}
