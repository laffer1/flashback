/* $Id: fbCriticalSection.h,v 1.2 2008/03/08 05:29:28 laffer1 Exp $ */

#ifndef fbCRITICALSECTION_H
#define fbCRITICALSECTION_H

#include "global.h"

/**
*	fbCriticalSection
*	Critical Section Object
*	@author Byron Heads
*	@date March 07, 2008
*	@note 3/5/08 - Created fbCriticalSection
*/
class fbCriticalSection
{
public:
	fbCriticalSection();	/// < Default constructor
	~fbCriticalSection();	/// < Destructor

	void lock();			/// < Lock the CS or block untill its unlocked
	void unlock();			/// < unlock the CS
	
	bool isLocked();		/// < Test if the CS is locked
	bool tryLock();			/// < Lock the CS but dont block if failed

private:
	bool _locked;			/// < True if Cs is locked

#ifdef Win32
	CRITICAL_SECTION hCriticalSection;
#else
	pthread_mutex_t	 hMutex;
#endif
};

#endif
