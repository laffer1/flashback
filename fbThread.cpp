/* $Id: fbThread.cpp,v 1.19 2008/04/18 03:33:55 laffer1 Exp $ */

/**
*	fbThread.cpp
*	fbThread Source
*	@author Byhron Heads
*	@date March 5, 2008
*/

#include <sys/param.h>
#include "fbThread.h"

/**
*	fbThread
*	Default constructor
*	@note 3/5/08 Initilize vars to false
*/
fbThread::fbThread(fbData* _data):_running(false), _stopping(false), _paused(false), data(_data), _hThread(0)
{
	data->debug(NONE, "fbThread.this");
}

/**
*	~fbThread
*	Destructor
*/
fbThread::~fbThread()
{
	if(_running)
		forceStop();
	data->debug(NONE, "fbThread.~this");
}

/**
*	start
*	starts the thread
*/
void fbThread::start()
{
	if(_running)
		return;

	data->debug(NONE, "fbThread.start");

#ifdef Win32
	_hThread = CreateThread(NULL, 0, threadStart, this, 0, NULL);
	if(_hThread == NULL)
		data->err(THREADCREATEFAIL, "CreateThread Failed");  // needs getlasterror
#else
	if(pthread_create(&_hThread, NULL, threadStart, this))
	{
		data->err(THREADCREATEFAIL, "pthread_create Failed");
	}
#endif
}


/**
*	start
*	starts the thread
*/
void fbThread::startDelete()
{
	if(_running)
		return;

	data->debug(NONE, "fbThread.startDelete");

#ifdef Win32
	_hThread = CreateThread(NULL, 0, threadStartDelete, this, 0, NULL);
	if(_hThread == NULL)
		data->err(THREADCREATEFAIL, "CreateThread Failed");  // needs getlasterror
#else
	if(pthread_create(&_hThread, NULL, threadStartDelete, this))
	{
		data->err(THREADCREATEFAIL, "pthread_create Failed");
	}
#endif
}

/**
*	stop
*	tell thread to stop
*/
void fbThread::stop()
{
	if (!_running)
		return;
	data->debug(NONE, "fbThread.stop");
	_stopping = true;
}

/**
*	forceStop
*	kills the thread, use with caution, may leave resources open
*/
void fbThread::forceStop()
{
	if (!_running)
		return;
#ifdef Win32
	if(TerminateThread(_hThread, 0) == -1)
		data->err(THREADTERMINATEFAILED, "TerminateThread Failed");
#else
	if(pthread_cancel(_hThread))
	{
		data->err(THREADTERMINATEFAILED, "pthread_cancel Failed");
	}
#endif
	_running = false;
	_stopping = false;
	_paused = false;
	_hThread = 0;
}

/**
*	pause
*	pause the thread
*/
void fbThread::pause()
{
	if (!_running || _paused)
		return;

#ifdef Win32
	if(SuspendThread(_hThread) == -1)
		data->err(THREADSUSPENDFAILED, "SuspendThread Failed");
#else
	return;
#endif
	_paused = true;
}

/**
*	resume
*	resume a _paused thread
*/
void fbThread::resume()
{
	if (!_running || !_paused)
		return;
#ifdef Win32
	if(ResumeThread(_hThread) == -1)
		data->err(THREADRESUMEFAILED, "ResumeThread Failed");
#else
	return;
#endif
	_paused = false;
}

/**
*	isRunning
*	is the thread running
*	@return true if thread is running
*/
bool fbThread::isRunning()
{
	return _running;
}

/**
*	ispaused
*	is the thread paused
*	@return true if thread is paused
*/
bool fbThread::isPaused()
{
	return _paused;
}

/**
*	isStopping
*	has the thread be asked to stop
*	@return true if thread is asked to stop
*/
bool fbThread::isStopping()
{
	return _stopping;
}


#ifdef Win32
/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
DWORD WINAPI fbThread::threadStart(LPVOID thread)
{
	fbThread* t = (fbThread*)thread;
	t->data->debug(NONE, "fbThread.threadStart");
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	t->_hThread = NULL;
	return 0;
}


DWORD WINAPI fbThread::threadStartDelete(LPVOID thread)
{
	fbThread* t = (fbThread*)thread;
	t->data->debug(NONE, "fbThread.threadStartDelete");
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	t->_hThread = NULL;
	delete t;
	return 0;
}
#else
/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
void* fbThread::threadStart(void* thread)
{
	fbThread* t = (fbThread*)thread;
	t->data->debug(NONE, "fbThread.threadStart");
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	pthread_detach(t->_hThread);
	t->_hThread = 0;
	return NULL;
}

/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
void* fbThread::threadStartDelete(void* thread)
{
	fbThread* t = (fbThread*)thread;
	t->data->debug(NONE, "fbThread.threadStartDelete");
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	pthread_detach(t->_hThread);
	t->_hThread = 0;
	t->data->debug(NONE, "fbThread.run delete myself");
	delete t;
	return NULL;
}
#endif

/**
*	sleep
*	Thread sleeps for given number of seconds
*	@param sec Number of seconds to sleep for
*/
void fbThread::_sleep(int sec)
{
	if(!_running || _paused)
		return;
#ifdef Win32
	Sleep(sec * 1000);
#else
	sleep(sec);
#endif
}

/**
*	usleep
*	Thread sleeps for given number of milliseconds
*	@param msec Number of milliseconds to sleep for
*/
void fbThread::_usleep(int msec)
{
	if(!_running || _paused)
		return;
#ifdef Win32
	Sleep(msec);
#else
	usleep(msec);
#endif
}

/**
*	yield
*	Thread yields allowing other threads to run
*/
void fbThread::_yield()
{
#ifdef Win32
	Yield();
#else
#ifndef NeXTBSD
	pthread_yield();
#endif
#endif
}

/**
*	run
*	virtual run function, overload for your own thread
*/
void fbThread::run()
{
}

