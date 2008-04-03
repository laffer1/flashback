/* $Id: fbRestore.cpp,v 1.2 2008/04/03 09:28:32 ctubbsii Exp $ */

#include "fbRestore.h"

fbRestore::fbRestore(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), from(src), to(dest)
{
    data->debug(NONE,"fbRestore.this");
    startDelete();
}

fbRestore::~fbRestore()
{
    data->debug(NONE,"fbRestore.~this");
}

void fbRestore::run()
{
    struct archive *a;
    struct archive_entry *entry;
    int resp;

    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_compression_bzip2(a);

    // change to destination directory
    if (chdir(to.c_str()) != 0)
    {
        data->err(NONE, "Can't change to destination directory to perform restore");
        return;
    }

    // open archive; requires absolute path, since we've already changed to destination directory
    if (archive_read_open_filename(a, from.c_str(), 10240))
    {
        data->err(NONE, archive_error_string(a));
        return;
    }

    while(true)
    {
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

        /* here, we want to make sure we strip leading slashes that would
           produce absolute paths, since we want to extract into a specific
           directory. needs some thought on this. */

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
    } // end while(true);

    archive_read_finish(a);
}
