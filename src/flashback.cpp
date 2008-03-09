/* $Id: flashback.cpp,v 1.9 2008/03/09 16:43:39 wyverex Exp $ */

#include "global.h"  /// < Holds Common Global Header Files

/**
 *	main
 *	Main program Entry Point
 *	@param argc number of input params
 *	@param args pointer to string array of input param
 *	@return returns program success
*/
int main(int argc, char** args)
{
#ifdef Debug
	cout << "Running in Debug Mode!" << endl;
#else
#ifndef WIN32	
			// TODO: make this Windows friendly	
	switch( fork() )
	{
		// DIDNT WORK
		case -1:
			perror("fork");
			return -1;
			break;

		// parent process returns value > 0 upon success (child id)
		default:
			return 0;

		case 0:
			// close unneeded file descriptors
			fclose(stdout);
			fclose(stdin);
			fclose(stderr);


			setsid(); // become session leader
			chdir("/"); // change wrkdir to root
			umask(0); // clear file mode creation mask

			break;
	}
#endif
#endif

	//Call Flashback Start
	

	return 0;
}	
 
