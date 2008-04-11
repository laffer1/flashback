/* $Id: fbBackup.cpp,v 1.8 2008/04/11 21:42:32 ctubbsii Exp $ */

#include "fbBackup.h"

fbBackup::fbBackup(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), path(src), filename(dest)
{
	data->debug(NONE,"fbBackup.this %s -> %s", path.c_str(), filename.c_str());
	startDelete();
}

fbBackup::~fbBackup()
{
	data->debug(NONE,"fbBackup.~this %s -> %s", path.c_str(), filename.c_str());
}


void fbBackup::run()
{
    struct archive *a = NULL;
    bool goAhead = true;
    int resp = 0;

    data->msg(NONE, "Backup begun: %s -> %s", path.c_str(), filename.c_str());

    a = archive_write_new();
    archive_write_set_format_ustar(a);
    // archive_write_set_bytes_per_block(a, 10240); // this is default
    archive_write_set_compression_bzip2(a); // could also use gzip

    resp = archive_write_open_filename(a, filename.c_str());
    if (resp != ARCHIVE_OK)
    {
        data->err(NONE, "Unable to create backup file: %d", resp);
        goAhead = false;
    }

    if (goAhead)
        traverseDir(&a, path.c_str());

    archive_write_finish(a);

    data->msg(NONE, "Backup complete: %s -> %s", path.c_str(), filename.c_str());

    /*
	string cmd = "tar -cf ";
	cmd += to;
	cmd += " ";
	cmd += to;

	system(cmd.c_str());
	*/
}

void fbBackup::traverseDir(struct archive **ap, const char *pathname)
{
    struct stat st;
    struct archive_entry *entry = NULL;
    int fd = 0;
    int buff[10240];
    size_t len = 0;
    int resp = 0;

    if (lstat(pathname, &st) < 0)
    {
        data->err(NONE, "Can't lstat pathname: %s", pathname);
        return;
    }

    entry = archive_entry_new();

    archive_entry_copy_stat(entry, &st);

    archive_entry_set_pathname(entry, pathname);
    fixAbsolutePaths(entry);

    if ((fd = open(pathname, O_RDONLY)) < 0)
    {
        data->err(NONE, "Can't open pathname for archiving: %d = open(%s)", fd, pathname);
        archive_entry_set_size(entry, 0);
    }

    if (!S_ISREG(st.st_mode))
    {
        data->debug(NONE, "%s is NOT a regular file!", pathname);
        /* archive_entry_set_size(entry, 0); */
    }

    archive_write_header(*ap, entry);
    // might want to check for success here

    while (S_ISREG(st.st_mode) && fd >= 0 && (len = read(fd, buff, sizeof(buff)) > 0))
    {
        data->debug(NONE, "Wrote %i bytes from %s", len, archive_entry_pathname(entry));
        resp = archive_write_data(*ap, buff, len);
        if (resp != ARCHIVE_OK)
            data->debug(NONE, "Problem writing data to archive for %s", archive_entry_pathname(entry));
    }

    data->debug(NONE, "Archived file: %s", archive_entry_pathname(entry));

    if (fd >= 0)
        close(fd);

    archive_entry_free(entry);

    // done archiving current file, now check if it's a directory, in order to recurse
    if (S_ISDIR(st.st_mode))
    {
        DIR *dir = NULL;
        struct dirent *item = NULL;
        char nextpath[PATH_MAX+1]; // leave room for \0

        if (!(dir = opendir(pathname)))
        {
            data->warn(NONE, "Can't descend into path %s", pathname);
            return;
        }

        while ((item = readdir(dir)) != NULL)
        {
            if (!strcmp(item->d_name, ".") || !strcmp(item->d_name, ".."))
                continue;

            if (pathname[strlen(pathname)-1] == PATH_NAME_SEPARATOR)
                snprintf(nextpath, sizeof(buff)-1, "%s%s", pathname, item->d_name);
            else
                snprintf(nextpath, sizeof(buff)-1, "%s%c%s", pathname, PATH_NAME_SEPARATOR, item->d_name);

            traverseDir(ap, nextpath);
        }

        closedir(dir);
    }
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
