/* $Id: fbHttpResponse.cpp,v 1.16 2008/03/29 18:25:13 wyverex Exp $ */
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

#include <sys/param.h>
#include <stdlib.h>

#include "fbHttpServer.h"
#include "fbHttpResponse.h"
#include "fbSocket.h"

#define MIMECOUNT 8

static const char *  mime[][2] = { 
    { ".html", "text/html" },
    { ".htm", "text/htm" },
    { ".png", "image/png" },
    { ".jpg", "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".gif", "image/gif" },
    { ".txt", "text/plain" },
    { ".css", "text/css" },
    { NULL, NULL }
};

fbHttpResponse:: fbHttpResponse(fbData * _data, fbClient * _client): fbThread(_data), data(_data),   client(_client)//, running(false)
{
    data->debug(NONE, "fbHttpResponse.this");
}

fbHttpResponse::~fbHttpResponse()
{
    if (isRunning())
        shutdown();

    data->debug(NONE, "fbHttpResponse.~this");
}

void fbHttpResponse::startup()
{
    data->debug(NONE, "fbHttpResponse.startup");

    if (isRunning())
        return;

    start();
}

void fbHttpResponse::shutdown()
{
    data->debug(NONE, "fbHttpResponse.shutdown");

    if (!isRunning())
        return;

    stop();
    //running = false; // stop it gracefully

    data->debug(NONE, "fbHttpResponse.shutdown() delete myself");
    delete this;
}

void fbHttpResponse::run()
{
     // we should probably check this during the request  while(!isStopping())
    char *path;
    path = client->getPath();

    data->debug(NONE, "fbHttpResponse.run");

    sendfile(path);

/*
    if ( strcmp(path, "/") == 0 )
        index();
    else if ( strcmp(path, "/index") == 0 )
        index();
    else
        notfound();
*/

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
    FILE *fp;
    string realp = "/usr/local/share/flashback/www/";
    int c;
    char *resolved;

    data->debug(NONE, "fbHttpResponse.sendfile");

    if (path != NULL)
        realp.append(path);
    else
        return; // TODO: Error logging and 404?

    resolved = (char *)calloc(PATH_MAX, sizeof(char));
    realpath(realp.c_str(), resolved);

    if (!*resolved)
    {
        notfound();
        return;
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
