/* $Id: fbHttpResponse.cpp,v 1.22 2008/04/07 14:12:46 laffer1 Exp $ */
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <stdlib.h>

#include "fbHttpServer.h"
#include "fbHttpResponse.h"
#include "fbSocket.h"

#define MIMECOUNT 12

static const char *  mime[][2] = { 
    { ".html", "text/html" },
    { ".htm", "text/htm" },
    { ".png", "image/png" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".gif", "image/gif" },
    { ".txt", "text/plain" },
    { ".css", "text/css" },
    { ".xml", "text/xml" },
    { ".xsl", "text/xml" },
    { ".xslt", "text/xml" },
    { ".tar", "multipart/x-tar" },
    { NULL, NULL }
};

fbHttpResponse:: fbHttpResponse(fbData * _data, fbClient * _client): fbThread(_data), data(_data),   client(_client), running(false)
{
    data->debug(NONE, "fbHttpResponse.this");
}

fbHttpResponse::~fbHttpResponse()
{
    if (running)
        shutdown();

    data->debug(NONE, "fbHttpResponse.~this");
}

void fbHttpResponse::startup()
{
    data->debug(NONE, "fbHttpResponse.startup");

    if (running)
        return;

    running = true;
    startDelete();
    
}

void fbHttpResponse::shutdown()
{
    if (!running) return;

    data->debug(NONE, "fbHttpResponse.shutdown");
    running = false; // stop it gracefully
}

void fbHttpResponse::run()
{
     // we should probably check this during the request  while(!isStopping())
    char *path;
    path = client->getPath();

    data->debug(NONE, "fbHttpResponse.run");

    /* deal with / and /index it shoudl access our default index.html */
    if ( strcmp(path, "/") == 0 || strcmp( path, "/index" ) == 0 ) 
    {
        free(path);
        path = strdup("/index.hml");
    }

    sendfile(path);

    data->debug(NONE, "fbHttpResponse.run() free path memory");
    // we're mallocing this elsewhere.
    if ( path != NULL )
       free( path );

    data->debug(NONE, "fbHttpResponse.run() delete client");
    delete client;

    shutdown();  // clean up 
}


void fbHttpResponse::sendfile( const char * path )
{
    FILE *fp;           // the file to send to the client
    string realp = data->getWebServerRootPath();
    int c;                  // an individual character we're going to write to stream
    char *resolved;  // The path after it has been tested with realpath
    struct stat st;  // the information about a file from lstat call

    data->debug(NONE, "fbHttpResponse.sendfile");

    if (path != NULL)
        realp.append(path);
    else
    {
        notfound();
        return; // TODO: Error logging
    }

    resolved = (char *)calloc(PATH_MAX, sizeof(char));
    if (resolved == NULL)
    {
        internal();
        return;
    }
    realpath(realp.c_str(), resolved);

   /* The file is not there */
    if (!*resolved)
    {
        notfound();
        return;
    }

    /* Find out if it's a symlink */
    if ( lstat( resolved, &st ) == -1 )
   {
       /* since the system call failed, let's assume we can't read the file. */
       free(resolved);
       notfound();
       return;
   }
   else
   {
       if (S_ISLNK(st.st_mode) )
       { 
          // this means it's a symlink which we don't support.
         // TODO: permissions error instead?
         free(resolved);
          notfound();
          return;
       }
   }

    if ( (fp = fopen( resolved, "r" ) ) == NULL ) {
        //data->msg(NONE, "fbHttpResponse: Unable to open file");
        //data->msg(NONE, resolved);
        free(resolved);
	notfound();
        return;
    }

    status( "200", "OK" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", matchmimetype(resolved) );
    header( "Content-Language", "en-US" );
    client->write("\r\n"); // end header section

    while ( (c = fgetc(fp)) != EOF && !ferror(fp))
    {
        if (c != EOF)
            client->write(c);
    }

    fclose(fp);
    free(resolved);
    data->debug(NONE, "fbHttpResponse.sendfile() done writing file");
}

void fbHttpResponse::notfound()
{
    string r;  // response for client
    char *path;

    data->debug(NONE, "fbHttpServer.notfound");

    path = client->getPath();

    status( "404", "Not Found" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    // TODO: Expires header

    r.append("\r\n"); // extra to start response as required per spec
    r.append( "<html>\n<head>\n\t<title>404 Not Found</title>\n</head>\n");
    r.append("<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on the server.</p>\n");
    r.append("<p>Invalid path: ");
    r.append(path);
    r.append("</p>\n<hr><p>");
    r.append(SERVERID);
    r.append("</p>\n</body>\n</html>\n");

    client->write(r);

    if (path != NULL)
        free( path );
}

void fbHttpResponse::internal()
{
    data->debug(NONE, "fbHttpServer.internal");
    status( "500", "Internal Server Error" );
    header( "Server", SERVERID );
    headdate();
    header( "Connection", "close");
    header( "Content-Type", "text/html; charset=iso-8859-1" );
    header( "Content-Language", "en-US" );
    client->write("\r\n");;
}

// ok so it's invalid for .9... 
void fbHttpResponse::status( string code, string msg )
{
    string r;
    r.append( "HTTP/1.0 " );
    r.append( code );
    r.append( " " );
    r.append( msg );
    r.append( "\r\n" );
    client->write(r);
}

void fbHttpResponse::header( string name, string value )
{
    string r;
    r.append(name);
    r.append(": ");
    r.append(value);
    r.append("\r\n");
    client->write(r);
}

void fbHttpResponse::headdate()
{
    struct tm *tm;
    time_t now;
    char date[50];
    string r;

    now = time( 0 );
    tm = gmtime( &now ); /* HTTP 1.1 spec rev 06 sez GMT only */
    strftime( date, 50, "%a, %d %b %Y %H:%M:%S GMT", tm );
    r.append(date);
    header( "Date", r );
}

const char * fbHttpResponse::matchmimetype( const char *filename )
{
    size_t len;
    size_t extlen;
    int i;

    len = strlen(filename);

    for ( i = 0; i < MIMECOUNT; i++ )
    {
        extlen = strlen( mime[i][0] );
        if (strcasecmp( mime[i][0], 
                 filename + (len - extlen)) == 0)
            return mime[i][1]; 
    }

    return "text/plain"; 
}
