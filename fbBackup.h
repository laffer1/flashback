/* $Id: fbBackup.h,v 1.11 2008/04/23 01:42:26 ctubbsii Exp $ */
/*
 * Copyright (c) 2008 Chris Tubbs
 * All rights reserved.
 * Do whatever you want with this code.
 *
*/
#ifndef fbBACKUP_H
#define fbBACKUP_H

#include "global.h"
#include "fbThread.h"
#include "fbData.h"
#include <archive.h>
#include <archive_entry.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/param.h>

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
    void fixPath(struct archive_entry *entry);
    void traverseDir(const string& pathname);
    void addFile(const string& pathname, struct stat *st);
};

#endif
