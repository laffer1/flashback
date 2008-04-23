/* $Id: fbRestore.h,v 1.4 2008/04/23 00:53:30 ctubbsii Exp $ */
/*
 * Copyright (c) 2008 Chris Tubbs
 * All rights reserved.
 * Do whatever you want with this code.
 *
*/
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
	string tarfile;
	string restorepath;

	void run();

};

#endif
