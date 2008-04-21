/* $Id: fbBackup.cpp,v 1.17 2008/04/21 15:02:35 ctubbsii Exp $ */
/*-
 * Copyright (C) 2008 Lucas Holt. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*-
 * Copyright (c) 2007 Chris Reinhardt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $MidnightBSD: src/lib/libmport/bundle.c,v 1.1 2008/01/05 22:18:20 ctriv Exp $
 */

#include <sys/param.h>
#include "fbBackup.h"

fbBackup::fbBackup(fbData* _data, const string& src, const string& dest):fbThread(_data), data(_data), backuppath(src), tarfile(dest), a(NULL)
{
	data->debug(NONE,"fbBackup.this %s -> %s", backuppath.c_str(), tarfile.c_str());
	startDelete();
}

fbBackup::~fbBackup()
{
	data->debug(NONE,"fbBackup.~this %s -> %s", backuppath.c_str(), tarfile.c_str());
}


void fbBackup::run()
{
    data->msg(NONE, "Backup begun: %s -> %s", backuppath.c_str(), tarfile.c_str());

    a = archive_write_new();
    data->debug(NONE, "a = archive_write_new()");
    archive_write_set_format_ustar(a);
    // archive_write_set_bytes_per_block(a, 10240); // this is default
    archive_write_set_compression_bzip2(a); // could also use gzip

    if (archive_write_open_filename(a, tarfile.c_str()) != ARCHIVE_OK)
    {
        data->debug(NONE, "case1: archive_write_open_filename(a, \"%s\")", tarfile.c_str());
        data->warn(NONE, "Unable to create backup file: %s", archive_error_string(a));
    }
    else
    {
        data->debug(NONE, "case2: archive_write_open_filename(a, \"%s\")", tarfile.c_str());
        traverseDir(backuppath);
    }

   if ( archive_write_finish(a) != ARCHIVE_OK)
       data->warn(NONE, "Unable to create backup file: %s", archive_error_string(a));
    data->debug(NONE, "archive_write_finish(a)");


    data->msg(NONE, "Backup complete: %s -> %s", backuppath.c_str(), tarfile.c_str());

    /*
	string cmd = "tar -cf ";
	cmd += to;
	cmd += " ";
	cmd += to;

	system(cmd.c_str());
	*/
}

void fbBackup::traverseDir(const string& pathname)
{
    struct stat st;

    data->debug(NONE, "traverseDir(\"%s\")", pathname.c_str());
    if (lstat(pathname.c_str(), &st) < 0)
    {
        data->warn(NONE, "Can't lstat pathname: %s", pathname.c_str());
        return;
    }
    data->debug(NONE, "lstat(\"%s\", &st)", pathname.c_str());

    // if it's a regular file or symlink, add it to the archive
    if (S_ISREG(st.st_mode)  || S_ISLNK(st.st_mode))
    {
        data->debug(NONE, "\t...is a regular file");
        addFile(pathname, &st);
    }
    // otherwise, if it's a directory, traverse it
    else if (S_ISDIR(st.st_mode))
    {
        DIR *dir;
        struct dirent *item;
        queue<string> morepaths;
        string nextpath;

        if (!(dir = opendir(pathname.c_str())))
        {
            data->warn(NONE, "Can't descend into path %s", pathname.c_str());
            return;
        }

        data->debug(NONE, "Descending into path %s", pathname.c_str());

        // get everything in this directory
        while ((item = readdir(dir)) != NULL)
        {
            nextpath = pathname;

            if (!strcmp(item->d_name, ".") || !strcmp(item->d_name, ".."))
                continue;

            // append a separator if one doesn't already exist
            if (nextpath[nextpath.length()-1] != PATH_NAME_SEPARATOR)
                nextpath += PATH_NAME_SEPARATOR;

            // append name of directory entry
            nextpath += item->d_name;

            // add it to the queue to check later, if it doesn't exceed the maximum length
            if (nextpath.length() < PATH_MAX)
                morepaths.push(nextpath);
        }
        closedir(dir);

        // while there is more in the directory, deal with it
        while (!morepaths.empty())
        {
            traverseDir(morepaths.front());
            morepaths.pop();
        }
    }
}

void fbBackup::addFile(const string& pathname, struct stat *st)
{
    data->debug(NONE, "addFile(\"%s\", struct stat *st)", pathname.c_str());
    struct archive_entry *entry = NULL;
    int fd = 0;
    int buff[1024 * 64];
    ssize_t len = 0;
    int resp;
    char *ent_path = strdup(pathname.c_str());

    if ((entry = archive_entry_new()) == NULL)
    {
        data->warn(NONE, "Cannot allocate memory for archive_entry_new()");
        return;
    }

    data->debug(NONE, "Allocated memory for archive_entry_new()");


    archive_entry_set_pathname(entry, ent_path);
    data->debug(NONE, "Completed archive_entry_set_pathname(entry, \"%s\")", ent_path);
    fixPath(entry);
    data->debug(NONE, "Fixed path to read: \"%s\")", archive_entry_pathname(entry));

    if (S_ISLNK(st->st_mode)) {
        /* we have us a symlink */
        int linklen;
        char linkdata[PATH_MAX];

        linklen = readlink(pathname.c_str(), linkdata, PATH_MAX);

        if (linklen < 0)
        {
            data->warn(NONE, "syscall failed");
            return;
        }

        linkdata[linklen] = 0;
        archive_entry_set_symlink(entry, linkdata);
    }

#ifdef BSD
    if (st->st_flags != 0)
        archive_entry_set_fflags(entry, st->st_flags, 0);
#endif

    archive_entry_copy_stat(entry, st);
    data->debug(NONE, "Completed archive_entry_copy_stat(entry, st)");

    if (!S_ISREG(st->st_mode))
    {
        data->debug(NONE, "%s is NOT a regular file!", pathname.c_str());
        archive_entry_set_size(entry, 0);
    }
    else if ((fd = open(pathname.c_str(), O_RDONLY)) < 0)
    {
        data->warn(NONE, "Can't open pathname for archiving: %d = open(%s)", fd, pathname.c_str());
        archive_entry_set_size(entry, 0);
    }

    resp = archive_write_header(a, entry);
    if (resp != ARCHIVE_OK)
    {
        data->debug(NONE, "Problem writing header for %s", pathname.c_str());
        return;
    }

    if (archive_entry_size(entry) > 0) {
        len = read(fd, buff, sizeof(buff));

        while (len > 0) {
             if (archive_write_data(a, buff, len) < 0)
                data->debug(NONE, "Problem writing data to archive for %s", archive_entry_pathname(entry));
            len = read(fd, buff, sizeof(buff));
        }
    }

    data->debug(NONE, "Archived file: %s", archive_entry_pathname(entry));

    if (fd >= 0)
        close(fd);

    archive_write_finish_entry(a);
  //  archive_write_finish(a);
    archive_entry_free(entry);
    free(ent_path);
}

void fbBackup::fixPath(struct archive_entry *entry)
{
	const char *name = archive_entry_pathname(entry);
	const char *root = backuppath.c_str();

    // trim off parts of name that correspond to original directory
    // tree. so, if we archive '/var/log', then '/var/log/messages'
    // will be archived as 'messages'
	while (name[0] == root[0])
	{
	    ++name;
	    ++root;
	}

    // strip leading slashes
	while (name[0] == PATH_NAME_SEPARATOR)
        ++name;

    // check for zero string length
	if (*name == '\0')
        name = ".";

	char *fixed = strdup(name);
	archive_entry_copy_pathname(entry, fixed);
	free(fixed);
}
