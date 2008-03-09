/* $Id: flashback.cpp,v 1.10 2008/03/09 18:02:12 laffer1 Exp $ */

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
	/* In UNIX programming, a daemon MUST let go of the terminal it is 
           running on.  Otherwise, when you log out, the process dies.
           The solution is to fork a copy of yourself and then disconnect
           ties to the original terminal like stdin, stdout, stderr
           The windows alternative would be whatever one must do to
           get a "service" in a microsoft product.
        */
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

			/* safety features */
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
 
