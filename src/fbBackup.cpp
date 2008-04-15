/* $Id: fbBackup.cpp,v 1.11 2008/04/15 02:11:16 ctubbsii Exp $ */

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

    archive_write_finish(a);
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

    // if it's a regular file, add it to the archive
    if (S_ISREG(st.st_mode))
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
    int buff[1024];
    ssize_t len = 0;
    int resp;

    if ((entry = archive_entry_new()) == NULL)
    {
        data->warn(NONE, "Cannot allocate memory for archive_entry_new()");
        return;
    }

    data->debug(NONE, "Allocated memory for archive_entry_new()");

    archive_entry_copy_stat(entry, st);
    data->debug(NONE, "Completed archive_entry_copy_stat(entry, st)");

    archive_entry_set_pathname(entry, pathname.c_str());
    data->debug(NONE, "Completed archive_entry_set_pathname(entry, \"%s\")", pathname.c_str());

    fixAbsolutePaths(entry);

    if ((fd = open(pathname.c_str(), O_RDONLY)) < 0)
    {
        data->warn(NONE, "Can't open pathname for archiving: %d = open(%s)", fd, pathname.c_str());
        archive_entry_set_size(entry, 0);
    }

    /* if (!S_ISREG(st.st_mode))
    {
        data->debug(NONE, "%s is NOT a regular file!", pathname.c_str());
        archive_entry_set_size(entry, 0);
    } */

    resp = archive_write_header(a, entry);
    if (resp != ARCHIVE_OK)
    {
        data->debug(NONE, "Problem writing header for %s", archive_entry_pathname(entry));
        return;
    }

    while (/* S_ISREG(st.st_mode) && */ fd >= 0 && (len = read(fd, buff, sizeof(buff)) > 0))
    {
        data->debug(NONE, "Wrote %i bytes from %s", len, archive_entry_pathname(entry));
        resp = archive_write_data(a, buff, len);
        if (resp != ARCHIVE_OK)
            data->debug(NONE, "Problem writing data to archive for %s", archive_entry_pathname(entry));
    }

    data->debug(NONE, "Archived file: %s", archive_entry_pathname(entry));

    if (fd >= 0)
        close(fd);

    archive_write_finish_entry(a);
    archive_entry_free(entry);
}

void fbBackup::fixAbsolutePaths(struct archive_entry *entry)
{
	const char *name = archive_entry_pathname(entry);

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
