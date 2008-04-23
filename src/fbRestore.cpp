/* $Id: fbRestore.cpp,v 1.8 2008/04/23 00:53:30 ctubbsii Exp $ */
/*
 * Copyright (c) 2008 Chris Tubbs
 * All rights reserved.
 * Do whatever you want with this code.
 *
*/
#include "fbRestore.h"

/* Constructor */
fbRestore::fbRestore(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), tarfile(src), restorepath(dest)
{
    data->debug(NONE,"fbRestore.this: %s -> %s", tarfile.c_str(), restorepath.c_str());
    startDelete();
}

/* Destructor */
fbRestore::~fbRestore()
{
    data->debug(NONE,"fbRestore.~this: %s -> %s", tarfile.c_str(), restorepath.c_str());
}

/* main thread activity */
void fbRestore::run()
{
    struct archive *a = NULL;
    struct archive_entry *entry = NULL;
    int resp = 0;

    data->msg(NONE, "Restore begun: %s -> %s", tarfile.c_str(), restorepath.c_str());

    /* don't restore if certain requirements are not met */
    if (!restorepath.length() || !tarfile.length())
    {
        data->warn(NONE, "Invalid restore attempted (restore source or destination empty)");
        return;
    }

    /* add pathname separator if one isn't already there */
    if (restorepath.at(restorepath.length()-1) != PATH_NAME_SEPARATOR)
        restorepath += PATH_NAME_SEPARATOR;

    /* create new archive structure and set reading options */
    a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_compression_bzip2(a);

    /* read the archive from the file */
    if (archive_read_open_filename(a, tarfile.c_str(), 10240))
    {
        data->warn(NONE, "Can't open archive file: %s", archive_error_string(a));
    }
    else
    {
        /* process the file if it's been opened */
        data->debug(NONE, "Archive opened");

        /* loop for each file in archive. exits when EOF or error */
        while(1)
        {
            const char *name = NULL;
            string newpath = restorepath;

            /* place next archive entry into variable named entry */
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
            if (resp == ARCHIVE_FATAL) /* give up */
            {
                data->warn(NONE, "Archive failed: %s", archive_error_string(a));
                break;
            }

            data->debug(NONE, "Attempting to extract file from archive: %s", archive_entry_pathname(entry));

            /* create a new path name for extracting to */
            name = archive_entry_pathname(entry);
            while (name[0] == PATH_NAME_SEPARATOR) /* strip leading slashes */
                ++name;
            newpath += name; /* append this filename to prefix (destination dir) */
            archive_entry_set_pathname(entry, newpath.c_str()); /* replace extraction name */
            data->debug(NONE, "Pathname changed to: %s", archive_entry_pathname(entry));

            /* reuse variable(resp) for extraction flags */
            resp = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_UNLINK |
                    ARCHIVE_EXTRACT_SECURE_SYMLINKS | ARCHIVE_EXTRACT_SECURE_NODOTDOT;

            /* extract the file */
            resp = archive_read_extract(a, entry, resp);
            data->debug(NONE, "Extracting file...");

            if (resp != ARCHIVE_OK)
                data->warn(NONE, "Archive not ok: %s", archive_error_string(a));
            if (resp == ARCHIVE_FAILED)
            {
                data->warn(NONE, "Archive failure");
                break;
            }

        } /* end while(true); */
    } /* end else */

    /* close the archive and clean up data structures */
    archive_read_finish(a);

    data->msg(NONE, "Restore complete: %s -> %s", tarfile.c_str(), restorepath.c_str());
}
