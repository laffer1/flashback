/* $Id: fbRestore.h,v 1.1 2008/04/03 07:56:43 ctubbsii Exp $ */

#ifndef fbRESTORE_H
#define fbRESTORE_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include <archive.h>
#include <archive_entry.h>

class fbRestore: public fbThread
{
public:
	fbRestore(fbData* _data, string& src, string& dest);
	~fbRestore();

private:
	fbData* data;
	string from;
	string to;

	void run();

};

#endif
