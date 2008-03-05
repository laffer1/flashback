

#ifndef fbTHREAD_H
#define fbTHREAD_H

#include "global.h"

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
	fbThread();			/// < Default Constructor	
	~fbThread();		/// < Destructor

	void start();		/// < starts the thread
	void stop();		/// < tell thread to stop
	void forceStop();	/// < force the thread to stop

	void pause();		/// < pause the thread, if running
	void resume();		/// < resumes a paused thread
	
	bool isRunning();	/// < is the thread running
	bool isPaused();	/// < is the thread paused
	bool isStopping();	/// < is the thread stopping?

private:
	bool running;		/// < is thread running
	bool stopping;			/// < did we get a stop?
	bool paused;		/// < has the thread been paused

	fbThread(const fbThread& thread);	/// < Suppress Copy Construct
	static void threadStart(void* thread);	/// < Real thread func

protected:
	virtual void run();	/// < Overload this function to run your thread in
};


#endif
