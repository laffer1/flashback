/* $Id */

#ifndef fbLOCK_H
#define fbLOCK_H

#include "global.h"
#include "fbCriticalSection.h"

/**
*	fbLock
*	Critical Section Object
*	@author Byron Heads
*	@date March 07, 2008
*	@note 3/5/08 - Created fbCriticalSection
*/
class fbLock
{
public:
	fbLock(fbCriticalSection& cs);	/// < Default constructor
	~fbLock();	/// < Destructor
private:
	fbCriticalSection* _cs;

};

#endif
