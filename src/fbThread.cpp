/**
*	fbThread.cpp
*	fbThread Source
*	@author Byhron Heads
*	@date March 5, 2008
*/

#include "fbThread.h"

/**
*	fbThread
*	Default constructor
*	@note 3/5/08 Initilize vars to false
*/
fbThread::fbThread():running(false), stopping(false), paused(false)
{
}

/**
*	~fbThread
*	Destructor
*/
fbThread::~fbThread()
{
	if(running)
		forceStop();
}

/**
*	start
*	starts the thread
*/
void fbThread::start()
{
	if(running)
		return;
}

/**
*	stop
*	tell thread to stop
*/
void fbThread::stop()
{
	if (!running)
		return;
	stopping = true;
}

/**
*	forceStop
*	kills the thread, use with caution, may leave resources open
*/
void fbThread::forceStop()
{
	if (!running)
		return;
}

/**
*	pause
*	pause the thread
*/
void fbThread::pause()
{
	if (!running)
		return;
}

/**
*	resume
*	resume a paused thread
*/
void fbThread::resume()
{
	if (!running || !paused)
		return;
}
	
/**
*	isRunning
*	is the thread running
*	@return true if thread is running
*/
bool fbThread::isRunning()
{
	return running;
}

/**
*	isPaused
*	is the thread paused
*	@return true if thread is paused
*/
bool fbThread::isPaused()
{
	return paused;
}

/**
*	isStopping
*	has the thread be asked to stop
*	@return true if thread is asked to stop
*/
bool fbThread::isStopping()
{
	return stopping;
}

/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
void fbThread::threadStart(void* thread)
{
	fbThread* t = (fbThread*)thread;
	t->running = true;
	t->run();	/// < cast thread and call run
	t->running = false;
}

/**
*	run
*	virtual run function, overload for your own thread
*/
void fbThread::run()
{
}
