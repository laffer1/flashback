/**
*	fbLock
*	Quick Critical Section Lock
*	@author Byron Heads
*	@date March 08, 2008
*/

#include "fbLock.h"

/**
*	fbLock
*	Default lock constructor
*	@note Gets a locks the CS
*/
fbLock::fbLock(fbCriticalSection& cs): _cs(&cs)
{
	_cs->lock();	/// < lock CS
}

/**
*	~fbLock
*	Destructor
*	@note Unlocks the CS
*/
fbLock::~fbLock()
{
	_cs->unlock();	/// < unlock CS
}
