/* $Id: fbRestore.cpp,v 1.5 2008/04/11 05:09:29 ctubbsii Exp $ */

#include "fbRestore.h"

fbRestore::fbRestore(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), filename(src), path(dest)
{
    data->debug(NONE,"fbRestore.this: %s -> %s", filename.c_str(), path.c_str());
    startDelete();
}

fbRestore::~fbRestore()
{
    data->debug(NONE,"fbRestore.~this: %s -> %s", filename.c_str(), path.c_str());
}

void fbRestore::run()
{
    struct archive *a = NULL;
    struct archive_entry *entry = NULL;
    bool goAhead = true;
    int resp = 0;

    data->msg(NONE, "Restore begun: %s -> %s", filename.c_str(), path.c_str());

    if (path.at(path.length()) != PATH_NAME_SEPARATOR)
        path += PATH_NAME_SEPARATOR;

    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_compression_bzip2(a);

    // change to destination directory
    /* if (chdir(path.c_str()) != 0)
    {
        data->err(NONE, "Can't change to destination directory to perform restore");
        goAhead = false;
    } */

    // open archive; requires absolute path, since we've already changed to destination directory
    /* else */ if (archive_read_open_filename(a, filename.c_str(), 10240))
    {
        data->err(NONE, archive_error_string(a));
        goAhead = false;
    }

    while(goAhead)
    {
        const char *name = NULL;
        string newpath;

        // place next archive entry into variable named entry
        resp = archive_read_next_header(a, &entry);

        if (resp == ARCHIVE_EOF)
            break;
        if (resp < ARCHIVE_OK)
            data->warn(NONE, archive_error_string(a));
        if (resp == ARCHIVE_RETRY) {
            data->warn(NONE, "Retrying last operation");
            continue;
        }
        if (resp == ARCHIVE_FATAL) // give up
        {
            data->err(NONE, archive_error_string(a));
            break;
        }

        data->debug(NONE, "Attempting to extract file from archive: ");
        data->debug(NONE, archive_entry_pathname(entry));

        // create a new path name for extracting to
        name = archive_entry_pathname(entry);
        while (name[0] == PATH_NAME_SEPARATOR) // strip leading
            ++name;
        newpath = path + name; // add to prefix (output dir)
        archive_entry_copy_pathname(entry, newpath.c_str()); // replace extraction name

        // reuse variable(resp) for extraction flags
        resp = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_UNLINK |
                ARCHIVE_EXTRACT_SECURE_SYMLINKS | ARCHIVE_EXTRACT_SECURE_NODOTDOT;

        resp = archive_read_extract(a, entry, resp);

        if (resp != ARCHIVE_OK)
            data->warn(NONE, archive_error_string(a));
        if (resp == ARCHIVE_FAILED)
        {
            data->err(NONE, "Archive failure");
            break;
        }

        archive_entry_free(entry);
    } // end while(true);

    archive_read_finish(a);

    data->msg(NONE, "Restore complete: %s -> %s", filename.c_str(), path.c_str());
}
