/* $Id: fbBackup.h,v 1.2 2008/04/03 07:56:43 ctubbsii Exp $ */

#ifndef fbBACKUP_H
#define fbBACKUP_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include <archive.h>
#include <archive_entry.h>

class fbBackup: public fbThread
{
public:
	fbBackup(fbData* _data, string& src, string& dest);
	~fbBackup();

private:
	fbData* data;
	string from;
	string to;

	void run();

};

#endif
