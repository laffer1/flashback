/* $Id: fbBackup.h,v 1.7 2008/04/11 23:49:30 ctubbsii Exp $ */

#ifndef fbBACKUP_H
#define fbBACKUP_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include <archive.h>
#include <archive_entry.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    void fixAbsolutePaths(struct archive_entry *entry);
    void traverseDir(struct archive *a, const char *pathname);
};

#endif
