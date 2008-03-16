/* $Id: fbRepeatType.h,v 1.1 2008/03/16 17:05:46 wyverex Exp $ */

#ifndef fbREPEATTYPE_H
#define fbREPEATTYPE_H

enum Repeat_type
{
	ONCE = 0,	/// < dont repeat, run once 
	MINS,		/// < repeat every x mins, 15 min intervals only!!!
	HOUR,		/// < repeat every x hours
	DAY,		/// < repeat every x days
	WEEK,		/// < repeat every x weeks
	MONTH,		/// < repeat every x month
	YEAR		/// < repeat every x years
};

#endif

