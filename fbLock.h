/* $Id: fbLock.h,v 1.3 2008/04/09 13:09:06 wyverex Exp $ */

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
