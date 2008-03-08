/* $Id: fbCriticalSection.cpp,v 1.2 2008/03/08 05:29:28 laffer1 Exp $ */

/**
*	fbCriticalSection
*	Creates a CS lock to prevent threads 
*	from reading and writing at the same time
*	@author Byron Heads
*	@date March 07, 2008
*/

#include "fbCriticalSection.h"

/**
*	fbCriticalSection
*	Default Critical Section constructor
*	@note Initilize all member vars
*/
fbCriticalSection::fbCriticalSection():_locked(false)
{
#ifdef Win32
	InitializeCriticalSection(&hCriticalSection);	/// < Create CS object
#else
	 pthread_mutex_init(&hMutex, NULL);
#endif
}

/**
*	~fbCriticalSection
*	Destructor, Delete CS object
*/
fbCriticalSection::~fbCriticalSection()
{
#ifdef Win32
	DeleteCriticalSection(&hCriticalSection);
#else
	pthread_mutex_destroy(&hMutex);
#endif
}

/**
*	lock
*	Locks the CS, or blocks thread till CS is unlocked
*/
void fbCriticalSection::lock()
{
#ifdef Win32
	EnterCriticalSection(&hCriticalSection);
#else
	 pthread_mutex_lock(&hMutex);
#endif
	_locked = true;	/// < remember CS is locked
}

/**
*	unlock
*	unlocks a locked CS and runs first block thread
*/
void fbCriticalSection::unlock()
{
	_locked = false;	/// < mark unlocked
#ifdef Win32
	LeaveCriticalSection(&hCriticalSection);
#else
	 pthread_mutex_unlock(&hMutex);
#endif	
}

/**
*	isLocked
*	Test if thread is currently locked
*	@return True is CS is locked
*/
bool fbCriticalSection::isLocked()
{
	return _locked;
}

/**
*	tryLock
*	Tries to lock CS, but doesn't block if locked
*	@return True if was able to get lock
*/
bool fbCriticalSection::tryLock()
{
#ifdef Win32
	if(!TryEnterCriticalSection(&hCriticalSection))
		return false;
#else
	if(!pthread_mutex_trylock(&hMutex))
		return false;
#endif
	_locked = true;
	return true;
}
