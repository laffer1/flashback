/* $Id: fbBackup.h,v 1.8 2008/04/13 22:45:46 ctubbsii Exp $ */

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
	fbBackup(fbData* _data, const string& src, const string& dest);
	~fbBackup();

private:
	fbData* data;
	string backuppath;
	string tarfile;
	struct archive *a;

	void run();
    void fixAbsolutePaths(struct archive_entry *entry);
    void traverseDir(const string& pathname);
    void addFile(const string& pathname, struct stat *st);
};

#endif
