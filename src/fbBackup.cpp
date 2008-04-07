/* $Id: fbBackup.cpp,v 1.3 2008/04/07 10:34:45 ctubbsii Exp $ */

#include "fbBackup.h"

fbBackup::fbBackup(fbData* _data, string& src, string& dest):fbThread(_data), data(_data), path(src), filename(dest)
{
	data->debug(NONE,"fbBackup.this");
	startDelete();
}

fbBackup::~fbBackup()
{
	data->debug(NONE,"fbBackup.~this");
}


void fbBackup::run()
{
    struct archive *a = NULL;
    bool goAhead = true;
    struct stat st;
    int resp = 0;

    a = archive_write_new();
    archive_write_set_format_ustar(a);
    // archive_write_set_bytes_per_block(a, 10240); // this is default
    archive_write_set_compression_bzip2(a); // could also use gzip

    resp = archive_write_open_filename(a, filename.c_str());
    if (resp != ARCHIVE_OK)
    {
        data->err(NONE, "Unable to create backup file");
        goAhead = false;
    }

    // don't add files yet... not quite completed
    /* while (nextfile)
    {
        stat(nextfile, &st);
        entry = archive_entry_new();
        archive_entry_copy_stat(entry, &st);
        archive_entry_set_pathname(entry, nextfile.c_char());
        archive_write_header(a, entry);
        fd = open(nextfile, O_RDONLY);
        while ((len = read(fd, buff, sizeof(buff))) > 0)
            archive_write_data(a, buff, len);
        archive_entry_free(entry);
        nextfile = getNextFile();
    } */

    archive_write_finish(a);

    /*
	string cmd = "tar -cf ";
	cmd += to;
	cmd += " ";
	cmd += to;

	system(cmd.c_str());
	*/
}
