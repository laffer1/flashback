/* $Id: fbBackup.h,v 1.5 2008/04/09 12:52:50 wyverex Exp $ */

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

#ifdef WIN32
#define PATH_NAME_SEPARATOR '\\'
#else
#define PATH_NAME_SEPARATOR '/'
#endif

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
    void traverseDir(struct archive **ap, const char *pathname);
};

#endif
