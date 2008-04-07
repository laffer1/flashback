/* $Id: fbBackup.h,v 1.3 2008/04/07 10:34:45 ctubbsii Exp $ */

#ifndef fbBACKUP_H
#define fbBACKUP_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include <archive.h>
#include <archive_entry.h>
#include <dirent.h>

class fbBackup: public fbThread
{
public:
	fbBackup(fbData* _data, string& src, string& dest);
	~fbBackup();

private:
	fbData* data;
	string path;
	string filename;

	void run();

};

#endif
