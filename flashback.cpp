/* $Id: flashback.cpp,v 1.20 2009/05/17 00:05:57 laffer1 Exp $ */

#include "global.h"  /// < Holds Common Global Header Files
void core();	     /// < flashback core running function 

#ifdef Win32

#define SVCNAME TEXT("FlashBack")

VOID SvcInstall(void);
VOID WINAPI SvcMain( DWORD, LPTSTR * );
VOID SvcReportEvent( LPTSTR );

//
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{ 
    // If command-line parameter is "install", install the service. 
    // Otherwise, the service is probably being started by the SCM.

    if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
        SvcInstall();
        return;
    }

    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.

    if (!StartServiceCtrlDispatcher( DispatchTable )) 
    { 
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); 
    } 
}

//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction) 
{ 
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];

    hEventSource = RegisterEventSource(NULL, SVCNAME);

    if( NULL != hEventSource )
    {
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

        lpszStrings[0] = SVCNAME;
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
} 
#else

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
#endif

	/* In UNIX programming, a daemon MUST let go of the terminal it is 
       running on.  Otherwise, when you log out, the process dies.
       
       The solution is to fork a copy of yourself and then disconnect
       ties to the original terminal like stdin, stdout, stderr
       The windows alternative would be whatever one must do to
       get a "service" in a microsoft product.
	*/


	int pid = fork();
	switch(pid)
	{
		// DIDNT WORK
		case -1:
#ifdef Debug
			std::cout << "Failed to fork" << endl;
#endif
			perror("fork");
			return -1;
			break;
		case 0:
#ifdef Debug
			std::cout << "Starting Deamon.." << endl;
#endif
			// close unneeded file descriptors
			fclose(stdout);
			fclose(stdin);
			fclose(stderr);

			// safety features *
			setsid(); // become session leader
			if (chdir("/") == -1) { // change wrkdir to root
				return 1;
			}
			umask(0); // clear file mode creation mask

			//Call Flashback Start
			core();
			break;
		// parent process returns value > 0 upon success (child id)
		default:
#ifdef Debug
			std::cout << "Deamon running.." << pid << endl;
#endif
			break;
	}
	return 0;
}	

#endif 
