/* $Id: fbRestore.h,v 1.2 2008/04/07 10:34:45 ctubbsii Exp $ */

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
	string filename;
	string path;

	void run();

};

#endif
