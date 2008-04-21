/* $Id: fbRestore.cpp,v 1.6 2008/04/21 15:02:35 ctubbsii Exp $ */

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
    int resp = 0;

    data->msg(NONE, "Restore begun: %s -> %s", filename.c_str(), path.c_str());

    if (path.at(path.length()-1) != PATH_NAME_SEPARATOR)
        path += PATH_NAME_SEPARATOR;

    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_compression_bzip2(a);

    if (archive_read_open_filename(a, filename.c_str(), 10240))
    {
        data->warn(NONE, "Can't open archive file: %s", archive_error_string(a));
    }
    else
    {
        data->debug(NONE, "Archive opened");
        while(1)
        {
            const char *name = NULL;
            string newpath = path;

            // place next archive entry into variable named entry
            entry = NULL;
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
                data->warn(NONE, "Archive failed: %s", archive_error_string(a));
                break;
            }

            data->debug(NONE, "Attempting to extract file from archive: %s", archive_entry_pathname(entry));

            // create a new path name for extracting to
            name = archive_entry_pathname(entry);
            while (name[0] == PATH_NAME_SEPARATOR) // strip leading
                ++name;
            newpath += name; // add to prefix (output dir)
            archive_entry_set_pathname(entry, newpath.c_str()); // replace extraction name
            data->debug(NONE, "Pathname changed to: %s", archive_entry_pathname(entry));

            // reuse variable(resp) for extraction flags
            resp = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_UNLINK |
                    ARCHIVE_EXTRACT_SECURE_SYMLINKS | ARCHIVE_EXTRACT_SECURE_NODOTDOT;

            resp = archive_read_extract(a, entry, resp);
            data->debug(NONE, "Extracting file...");

            if (resp != ARCHIVE_OK)
                data->warn(NONE, "Archive not ok: %s", archive_error_string(a));
            if (resp == ARCHIVE_FAILED)
            {
                data->warn(NONE, "Archive failure");
                break;
            }

        } // end while(true);
    } // end else

    archive_read_finish(a);

    data->msg(NONE, "Restore complete: %s -> %s", filename.c_str(), path.c_str());
}
