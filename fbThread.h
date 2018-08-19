/* $Id: fbThread.h,v 1.10 2008/03/29 19:05:26 wyverex Exp $ */

#ifndef fbTHREAD_H
#define fbTHREAD_H

#include "global.h"
#include "fbData.h"

/**
*	fbThread
*	Base Thread object
*	@author Byron Heads
*	@date March 05, 2008
*	@note 3/5/08 - Created fbThread
*/
class fbThread
{
public:
	fbThread(fbData* _data); /// < Overloaded Constructor, ErrorLogger
	virtual ~fbThread();		 /// < Destructor

	void start();		/// < starts the thread
	void startDelete();		/// < starts the thread
	void stop();		/// < tell thread to stop
	void forceStop();	/// < force the thread to stop

	void pause();		/// < pause the thread, if running
	void resume();		/// < resumes a paused thread
	
	bool isRunning();	/// < is the thread running
	bool isPaused();	/// < is the thread paused
	bool isStopping();	/// < is the thread stopping?

	void _sleep(int sec);  	/// < thread sleeps for the given number of seconds
	void _usleep(int msec); /// < thread sleeps for the given number of milliseconds
	void _yield();		/// < thread yieids for other threads to run
	
private:
	bool _running;		/// < is thread running
	bool _stopping;		/// < did we get a stop?
	bool _paused;		/// < has the thread been paused
	fbData* data;	/// < Threads Error Logger

#ifdef Win32
	HANDLE _hThread;		/// < handle to running thread
	static DWORD WINAPI threadStart(LPVOID thread);	/// < Real thread func
	static DWORD WINAPI threadStartDelete(LPVOID thread);	/// < Real thread func, deletes thread when done
#else
	pthread_t _hThread;		/// < handle to running thread
	static void* threadStart(void* thread);	/// < Real thread func
	static void* threadStartDelete(void* thread);	/// < Real thread func, deletes thread when done
#endif
	
	virtual void run();		/// < Overload this function to run your thread in
};


#endif

/*
//example usage

#include "fbThread.h"
class foo: public fbThread
{
private:
	void run()
	{
		cout << "Thread Starting.." << endl;
		while(!isStopping())
		{
			sleep(1);
			cout << "Still.. running!" << endl;
		}
		cout << "Thread stopping.." << endl;
	}
};

*/
